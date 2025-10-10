// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "ActInputConfig.h"

#include "ActInputAnalyzerSubsystem.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCommandMatchedDelegate, const FGameplayTag)

template<typename T, int32 Capacity>
struct TFixedRingBuffer
{
	T Data[Capacity];
	int32 Head = 0;
	int32 NumItems = 0;
    
	FORCEINLINE void Add(const T& Item)
	{
		Data[Head] = Item;
		Head = (Head + 1) % Capacity;
		NumItems = FMath::Min(NumItems + 1, Capacity);
	}

	FORCEINLINE void Empty()
	{
		Head = 0;
		NumItems = 0;
	}

	FORCEINLINE int32 GetNum() const { return NumItems; }
	FORCEINLINE bool IsEmpty() const { return NumItems == 0; }
	FORCEINLINE bool IsFull() const { return NumItems == Capacity; }
    
	FORCEINLINE int32 GetRealIndexFromOldest(const int32 LogicalIndex) const
	{
		check(LogicalIndex >= 0 && LogicalIndex < NumItems);
		return (Head - NumItems + LogicalIndex + Capacity) % Capacity;
	}

	FORCEINLINE int32 GetRealIndexFromLatest(const int32 OffsetFromLatest) const
	{
		check(OffsetFromLatest >= 0 && OffsetFromLatest < NumItems);
		return (Head - 1 - OffsetFromLatest + Capacity) % Capacity;
	}
    
	FORCEINLINE T& GetOldest(const int32 LogicalIndex = 0)
	{
		return Data[GetRealIndexFromOldest(LogicalIndex)];
	}

	FORCEINLINE T& GetLatest(const int32 Offset = 0)
	{
		return Data[GetRealIndexFromLatest(Offset)];
	}

	FORCEINLINE T& operator[](const int32 Index)
	{
		return GetOldest(Index);
	}
    
	FORCEINLINE void ToArray(TArray<T>& OutArray, const bool bFromOldest = true)
	{
		OutArray.Reserve(NumItems);

		if (bFromOldest)
		{
			for (int32 i = 0; i < NumItems; ++i)
			{
				OutArray.Add(GetOldest(i));
			}
		}
		else
		{
			for (int32 i = 0; i < NumItems; ++i)
			{
				OutArray.Add(GetLatest(i));
			}
		}
	}
};

struct FActInputBufferEntry
{
public:
	FActInputBufferEntry(){}

	explicit FActInputBufferEntry(const EActInputFlag InInputFlag, const bool InMatchTimeout = false) : InputFlag(InInputFlag), bMatchTimeout(InMatchTimeout) {}

public:
	void SetMatchTimeout(const bool InMatchTimeout) { bMatchTimeout = InMatchTimeout; }
	
public:
	EActInputFlag InputFlag = EActInputFlag::None;
	bool bMatchTimeout = false;
};

/**
 * 
 */
UCLASS()
class ACTGAME_API UActInputAnalyzerSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:
	void ResetInputAnalyzer(const UActInputCommandConfig* CommandConfig);
	
	void TickAnalyzer(const float DeltaSeconds);
	
	void AddInputFlagToBuffer(const EActInputFlag InputFlag);

	void ClearInputBuffer();

	void GetInputBuffer(TArray<FActInputBufferEntry>& OutArray);

	void AddOnScreenDebugMessageForBuffer();
	
private:
	void TryToMatchInputCommands();

	void UpdateBufferLifeTime(const float DeltaSeconds);

public:
	FOnCommandMatchedDelegate OnCommandMatched;

	
private:
	UPROPERTY()
	TArray<FActInputCommandSet> CommandSetContainer;
	
	TFixedRingBuffer<FActInputBufferEntry, 20> InputBuffer;
	
	UPROPERTY()
	float BufferLifeTime = 0.f;
};
