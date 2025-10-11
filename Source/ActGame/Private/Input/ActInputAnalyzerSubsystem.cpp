// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/ActInputAnalyzerSubsystem.h"

void UActInputAnalyzerSubsystem::InitInputAnalyzer(const UActInputCommandConfig* CommandConfig)
{
	CommandSetContainer = CommandConfig->CommandSetContainer;
	ClearInputBuffer();
}

void UActInputAnalyzerSubsystem::AddInputFlagToBuffer(const EActInputFlag InputFlag)
{
	const double WorldTimeSeconds = GetWorld()->GetTimeSeconds();
	InputBuffer.Add(FActInputBufferEntry(InputFlag, WorldTimeSeconds));
	TryToMatchInputCommands();
}

void UActInputAnalyzerSubsystem::ClearInputBuffer()
{
	if (!InputBuffer.IsEmpty())
	{
		InputBuffer.Empty();
	}
}

void UActInputAnalyzerSubsystem::GetInputBuffer(TArray<FActInputBufferEntry>& OutArray, const bool bFromOldest)
{
	InputBuffer.ToArray(OutArray, bFromOldest);
}

void UActInputAnalyzerSubsystem::AddOnScreenDebugMessageForBuffer()
{
	if (!GEngine) return;

	const UEnum* EnumObject = StaticEnum<EActInputFlag>();
	if (!EnumObject) return;

	for (int32 i = 0; i < InputBuffer.GetNum(); ++i)
	{
		const EActInputFlag& InputFlag = InputBuffer[i].InputFlag;
		const double InputTime = InputBuffer[i].InputTime;
		
		FString Message = FString::Printf(TEXT("BufferIndex: [%d] : InputFlag: %s : InputTime: %f"),i,*EnumObject->GetNameStringByValue(static_cast<int64>(InputFlag)), InputTime);
		GEngine->AddOnScreenDebugMessage(i,0.f, FColor::Blue,Message);
	}
}

void UActInputAnalyzerSubsystem::TryToMatchInputCommands()
{
	if (InputBuffer.IsEmpty())
	{
		return;
	}
	
	FGameplayTag ResultTag;
	
	for (const FActInputCommandSet& CommandSet : CommandSetContainer)
	{
		const TArray<FActInputCommandEntry>& CommandEntrySet = CommandSet.CommandEntrySet;

		int32 BufferIndex = InputBuffer.GetNum() -1;
		int32 EntrySetIndex = CommandEntrySet.Num() -1;

		double LastInputTime = -1;
		
		while (BufferIndex >= 0 && EntrySetIndex >= 0)
		{
			const FActInputBufferEntry& BufferEntry = InputBuffer.GetLatest(InputBuffer.GetNum() - 1 - BufferIndex);
			
			const EActInputFlag& BufferFlag = BufferEntry.InputFlag;
			const FActInputCommandEntry& CommandEntry = CommandEntrySet[EntrySetIndex];

			if (LastInputTime >= 0.f)
			{
				if (const float DeltaTime = LastInputTime - BufferEntry.InputTime; DeltaTime > CommandEntry.AllowedTimeGap)
				{
					break;
				}
			}
			
			if (BufferFlag != CommandEntry.InputFlag)
			{
				break;
			}

			--BufferIndex;
			--EntrySetIndex;

			LastInputTime = BufferEntry.InputTime;;
			
			if (EntrySetIndex < 0)
			{
				ResultTag = CommandSet.CommandTag;
				break;
			}
		}
	}

	if (ResultTag.IsValid())
	{
		OnCommandMatched.Broadcast(ResultTag);
	}
}
