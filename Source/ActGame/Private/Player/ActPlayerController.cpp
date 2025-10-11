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
	check(InputConfig)

	TArray<uint32> BindHandles;
	ActInputComponent->BindNativeInputFlagActions(InputConfig, this, &ThisClass::ActInputFlagPressed, &ThisClass::ActInputFlagTriggered, &ThisClass::ActInputFlagReleased, BindHandles);

	InputAnalyzerSubsystem = ULocalPlayer::GetSubsystem<UActInputAnalyzerSubsystem>(GetLocalPlayer());
	
	check(InputAnalyzerSubsystem);
	check(InputCommandConfig)

	InputAnalyzerSubsystem->InitInputAnalyzer(InputCommandConfig);
	InputAnalyzerSubsystem->OnCommandMatched.AddLambda(
		[](const FGameplayTag ResultTag)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, *ResultTag.ToString());
		}
	);
}

void AActPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	InputAnalyzerSubsystem->AddOnScreenDebugMessageForBuffer();
}

void AActPlayerController::ActInputFlagPressed(EActInputFlag InputFlag)
{
	InputAnalyzerSubsystem->AddInputFlagToBuffer(InputFlag);
}

void AActPlayerController::ActInputFlagTriggered(EActInputFlag InputFlag)
{
	
}

void AActPlayerController::ActInputFlagReleased(EActInputFlag InputFlag)
{
	
}
