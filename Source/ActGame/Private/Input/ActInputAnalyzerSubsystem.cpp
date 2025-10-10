// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/ActInputAnalyzerSubsystem.h"

void UActInputAnalyzerSubsystem::ResetInputAnalyzer(const UActInputCommandConfig* CommandConfig)
{
	CommandSetContainer = CommandConfig->CommandSetContainer;
	ClearInputBuffer();
}

void UActInputAnalyzerSubsystem::TickAnalyzer(const float DeltaSeconds)
{
	UpdateBufferLifeTime(DeltaSeconds);
}

void UActInputAnalyzerSubsystem::AddInputFlagToBuffer(const EActInputFlag InputFlag)
{
	
	InputBuffer.Add(FActInputBufferEntry(InputFlag));
	TryToMatchInputCommands();
}

void UActInputAnalyzerSubsystem::ClearInputBuffer()
{
	if (!InputBuffer.IsEmpty())
	{
		InputBuffer.Empty();
	}
}

void UActInputAnalyzerSubsystem::GetInputBuffer(TArray<FActInputBufferEntry>& OutArray)
{
	InputBuffer.ToArray(OutArray);
}

void UActInputAnalyzerSubsystem::AddOnScreenDebugMessageForBuffer()
{
	if (!GEngine) return;

	const UEnum* EnumObject = StaticEnum<EActInputFlag>();
	if (!EnumObject) return;

	for (int32 i = 0; i < InputBuffer.GetNum(); ++i)
	{
		const EActInputFlag& InputFlag = InputBuffer[i].InputFlag;
		const bool bMatchTimeout = InputBuffer[i].bMatchTimeout;
		
		FString Message = FString::Printf(TEXT("BufferIndex: [%d] : InputFlag: %s : MatchTimeout: %d"),i,*EnumObject->GetNameStringByValue(static_cast<int64>(InputFlag)), bMatchTimeout);
		GEngine->AddOnScreenDebugMessage(i,0.f, FColor::Blue,Message);
	}
}

void UActInputAnalyzerSubsystem::TryToMatchInputCommands()
{
	if (InputBuffer.IsEmpty())
	{
		return;
	}

	if (InputBuffer.GetLatest().bMatchTimeout)
	{
		return;
	}
	
	FGameplayTag ResultTag;
	
	for (const FActInputCommandSet& CommandSet : CommandSetContainer)
	{
		const TArray<FActInputCommandEntry>& CommandEntrySet = CommandSet.CommandEntrySet;

		int32 BufferIndex = InputBuffer.GetNum() -1;
		int32 EntrySetIndex = CommandEntrySet.Num() -1;

		while (BufferIndex >= 0 && EntrySetIndex >= 0)
		{
			const FActInputBufferEntry& BufferEntry = InputBuffer.GetLatest(InputBuffer.GetNum() - 1 - BufferIndex);
			
			const EActInputFlag& BufferFlag = BufferEntry.InputFlag;
			const FActInputCommandEntry& CommandEntry = CommandEntrySet[EntrySetIndex];

			if (BufferFlag != CommandEntry.InputFlag)
			{
				break;
			}

			--BufferIndex;
			--EntrySetIndex;

			BufferLifeTime = CommandEntry.AllowedTimeGap;

			if (EntrySetIndex < 0)
			{
				ResultTag = CommandSet.CommandTag;
			}
		}
	}

	if (ResultTag.IsValid())
	{
		OnCommandMatched.Broadcast(ResultTag);
	}
}

void UActInputAnalyzerSubsystem::UpdateBufferLifeTime(const float DeltaSeconds)
{
	if (BufferLifeTime > 0 && !InputBuffer.IsEmpty())
	{
		BufferLifeTime -= DeltaSeconds;
		BufferLifeTime = FMath::Max(BufferLifeTime, 0.0f);

		if (BufferLifeTime <= 0.0f)
		{
			FActInputBufferEntry& LatestEntry = InputBuffer.GetLatest();
			LatestEntry.bMatchTimeout = true;
		}
	}
}
