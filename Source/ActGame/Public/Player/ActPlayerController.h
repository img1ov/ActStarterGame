// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Input/ActInputConfig.h"

#include "ActPlayerController.generated.h"

enum class EActInputFlag : uint8;
class UActInputConfig;
class UActInputComponent;

USTRUCT(BlueprintType)
struct FActInputEntry
{
	GENERATED_BODY()

public:
	FActInputEntry(){}
	
	FActInputEntry(const EActInputFlag& InInputFlag) : InputFlag(InInputFlag) {}
	
public:
	UPROPERTY(BlueprintReadOnly)
	EActInputFlag InputFlag = EActInputFlag::None;

	UPROPERTY(EditDefaultsOnly)
	float TriggerDuration = 0.f;

	UPROPERTY(EditDefaultsOnly)
	uint8 bCompleted = false;
};

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

private:
	virtual void Tick(float DeltaSeconds) override;
	
	void ActInputFlagTriggered(EActInputFlag InputFlag);

	void ActInputFlagReleased(EActInputFlag InputFlag);

	
public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UActInputConfig> ActInputConfig;

private:
	TArray<FActInputEntry> ActInputEntryBuffer;
};
