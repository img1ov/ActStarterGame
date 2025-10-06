// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "ActInputConfig.generated.h"

class UInputAction;

UENUM(BlueprintType)
enum class EActInputFlag : uint8
{
	None,
	
	AttackLight,
	AttackLightHold,
	AttackHeavy,
	AttackHeavyHold,

	Jump,
	Dash,
};

USTRUCT(BlueprintType)
struct FActInputFlagAction
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	EActInputFlag InputFlag = EActInputFlag::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UInputAction> InputAction = nullptr;
};

USTRUCT(BlueprintType)
struct FActInputCommandEntry
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EActInputFlag CommandFlag = EActInputFlag::None;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float AllowedTimeGap = 0.25f;
};

USTRUCT(BlueprintType)
struct FActInputCommandSet
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag CommandTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FActInputCommandEntry> CommandEntrySet;
};

/**
 * UActInputConfig
 *
 *	Non-mutable data asset that contains input configuration properties.
 */
UCLASS()
class ACTGAME_API UActInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UActInputConfig(const FObjectInitializer& ObjectInitializer);

	const UInputAction* FindNativeInputActionForFlag(const EActInputFlag& InputFlag, bool bLogNotFound = true) const;


public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FActInputFlagAction> NativeInputFlagActions;
};

UCLASS()
class ACTGAME_API UActInputCommandConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UActInputCommandConfig(const FObjectInitializer& ObjectInitializer);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FActInputCommandSet> CommandSetContainer;
};