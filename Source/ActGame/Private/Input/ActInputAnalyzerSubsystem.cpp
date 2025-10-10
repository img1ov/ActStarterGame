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
	InputBuffer.Add(InputFlag);
	TryToMatchInputCommands();
}

void UActInputAnalyzerSubsystem::ClearInputBuffer()
{
	if (!InputBuffer.IsEmpty())
	{
		InputBuffer.Empty();
	}
}

void UActInputAnalyzerSubsystem::GetInputBuffer(TArray<EActInputFlag>& OutArray) const
{
	InputBuffer.ToArray(OutArray);
}

void UActInputAnalyzerSubsystem::AddOnScreenDebugMessageForBuffer() const
{
	if (!GEngine) return;

	const UEnum* EnumObject = StaticEnum<EActInputFlag>();
	if (!EnumObject) return;

	for (int32 i = 0; i < InputBuffer.Num(); ++i)
	{
		const EActInputFlag& InputFlag = InputBuffer[i];

		FString Message = FString::Printf(TEXT("[%d] : %s"),i,*EnumObject->GetNameStringByValue(static_cast<int64>(InputFlag)));
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

		int32 BufferIndex = InputBuffer.Num() -1;
		int32 EntrySetIndex = CommandEntrySet.Num() -1;

		while (BufferIndex >= 0 && EntrySetIndex >= 0)
		{
			const EActInputFlag& BufferFlag = InputBuffer.GetLatest(InputBuffer.Num() - 1 - BufferIndex);
			const FActInputCommandEntry& CommandEntry = CommandEntrySet[EntrySetIndex];

			if (BufferFlag != CommandEntry.CommandFlag)
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
			ClearInputBuffer();
		}
	}
}
