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

	
}

void AActPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UActInputComponent* ActInputComponent = CastChecked<UActInputComponent>(InputComponent);
	check(ActInputComponent)
	check(ActInputConfig)

	TArray<uint32> BindHandles;
	ActInputComponent->BindNativeInputFlagActions(ActInputConfig, this, &ThisClass::ActInputFlagTriggered, &ThisClass::ActInputFlagReleased, BindHandles);

	InputAnalyzerSubsystem = ULocalPlayer::GetSubsystem<UActInputAnalyzerSubsystem>(GetLocalPlayer());
	check(InputAnalyzerSubsystem)
}

void AActPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	InputAnalyzerSubsystem->AddDebugMassageOnScreen();
}

void AActPlayerController::ActInputFlagTriggered(EActInputFlag InputFlag)
{
	InputAnalyzerSubsystem->PushInputEntry(InputFlag);
}

void AActPlayerController::ActInputFlagReleased(EActInputFlag InputFlag)
{
	InputAnalyzerSubsystem->ReleaseInputEntry(InputFlag);
}
