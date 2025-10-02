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
	
	Attack_Light,
	Attack_Heavy,
	
	Jump,
};

/**
 * FActInputFlagAction
 *
 *	Struct used to map a input action to a act input flag.
 */
USTRUCT(BlueprintType)
struct FActInputFlagAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EActInputFlag InputFlag = EActInputFlag::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UInputAction> InputAction = nullptr;
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
	// List of input actions used by the owner.  These input actions are mapped to a act input flag and are automatically bound to matching ability commands.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
	TArray<FActInputFlagAction> NativeInputFlagActions;
};
