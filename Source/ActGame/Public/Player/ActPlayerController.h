// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Input/ActInputConfig.h"

#include "ActPlayerController.generated.h"

class UActInputAnalyzerSubsystem;
class UActInputComponent;

/**
 * 
 */
UCLASS()
class ACTGAME_API AActPlayerController : public AModularPlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	virtual void Tick(float DeltaSeconds) override;

private:
	void ActInputFlagPressed(EActInputFlag InputFlag);
	
	void ActInputFlagTriggered(EActInputFlag InputFlag);

	void ActInputFlagReleased(EActInputFlag InputFlag);

private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UActInputConfig> InputConfig;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UActInputCommandConfig> InputCommandConfig;

	UPROPERTY()
	TObjectPtr<UActInputAnalyzerSubsystem> InputAnalyzerSubsystem;
};
