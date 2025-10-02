// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EnhancedInputComponent.h"
#include "ActInputConfig.h"

#include "ActInputComponent.generated.h"

class UEnhancedInputLocalPlayerSubsystem;
class UInputAction;

/**
 * ActInputComponent
 *
 *	Component used to manage input mappings and bindings using an input config data asset.
 */
UCLASS()
class ACTGAME_API UActInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:

	UActInputComponent(const FObjectInitializer& ObjectInitializer);

	void AddInputMappings(const UActInputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const;
	void RemoveInputMappings(const UActInputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const;

	template<class UserClass, typename TriggerFuncType, typename ReleasedFuncType>
	void BindNativeInputFlagActions(const UActInputConfig* InputConfig, UserClass* Object, TriggerFuncType TriggerFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles);
	
	void RemoveBinds(TArray<uint32>& BindHandles);
	
};

template<class UserClass, typename TriggerFuncType, typename ReleasedFuncType>
void UActInputComponent::BindNativeInputFlagActions(const UActInputConfig* InputConfig, UserClass* Object, TriggerFuncType TriggerFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles)
{
	check(InputConfig);

	for (const FActInputFlagAction& Action : InputConfig->NativeInputFlagActions)
	{
		if (Action.InputAction)
		{
			if (TriggerFunc)
			{
				BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, TriggerFunc, Action.InputFlag).GetHandle());
			}

			if (ReleasedFunc)
			{
				BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputFlag).GetHandle());
			}
		}
	}
}
