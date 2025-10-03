// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ActPlayerController.h"
#include "Input/ActInputAnalyzerSubsystem.h"

#include "Input/ActInputComponent.h"

void AActPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* InputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		InputLocalPlayerSubsystem->AddMappingContext(InputMappingContext, 0);
	}

	InputAnalyzerSubsystem = ULocalPlayer::GetSubsystem<UActInputAnalyzerSubsystem>(GetLocalPlayer());
	check(InputAnalyzerSubsystem)
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
	
	for (const FActInputEntry& Entry : InputAnalyzerSubsystem->GetInputEntryBuffer())
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
	InputAnalyzerSubsystem->PushInputEntry(InputFlag);
}

void AActPlayerController::ActInputFlagReleased(EActInputFlag InputFlag)
{
	InputAnalyzerSubsystem->ReleaseInputEntry(InputFlag);
}
