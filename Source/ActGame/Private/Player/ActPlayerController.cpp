// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ActPlayerController.h"

#include "Input/ActInputComponent.h"

void AActPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* InputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		InputLocalPlayerSubsystem->AddMappingContext(InputMappingContext, 0);
	}

	
}

void AActPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UActInputComponent* ActInputComponent = CastChecked<UActInputComponent>(InputComponent);
	check(ActInputComponent)
	check(ActInputConfig)

	TArray<uint32> BindHandles;
	ActInputComponent->BindNativeInputFlagActions(ActInputConfig, this, &ThisClass::ActInputFlagTriggered, &ThisClass::ActInputFlagReleased, BindHandles);
}

void AActPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	for (FActInputEntry& Entry : ActInputEntryBuffer)
	{
		if (const UEnum* EnumObject = StaticEnum<EActInputFlag>())
		{
			GEngine->AddOnScreenDebugMessage(static_cast<int>(Entry.InputFlag), 2.f, FColor::Blue, FString::Printf(
				TEXT("%s : %f : %d"), *EnumObject->GetNameStringByValue(static_cast<int64>(Entry.InputFlag)), Entry.TriggerDuration, Entry.bCompleted));
		}
	}
}

void AActPlayerController::ActInputFlagTriggered(EActInputFlag InputFlag)
{
	if (ActInputEntryBuffer.IsEmpty())
	{
		ActInputEntryBuffer.Add(FActInputEntry(InputFlag));
		return;
	}
	
	FActInputEntry& TrailEntry = ActInputEntryBuffer.Last();
	if (TrailEntry.InputFlag == InputFlag && !TrailEntry.bCompleted)
	{
		TrailEntry.TriggerDuration += GetWorld()->GetDeltaSeconds();
	}
	else
	{
		ActInputEntryBuffer.Add(FActInputEntry(InputFlag));
	}
}

void AActPlayerController::ActInputFlagReleased(EActInputFlag InputFlag)
{
	for (int32 i = ActInputEntryBuffer.Num() - 1; i >= 0; --i)
	{
		FActInputEntry& Entry = ActInputEntryBuffer[i];
		if (Entry.InputFlag == InputFlag && !Entry.bCompleted)
		{
			Entry.bCompleted = true;
			break;
		}
	}
}
