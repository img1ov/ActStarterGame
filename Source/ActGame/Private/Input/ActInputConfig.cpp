// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/ActInputConfig.h"

UActInputConfig::UActInputConfig(const FObjectInitializer& ObjectInitializer)
{
}

const UInputAction* UActInputConfig::FindNativeInputActionForFlag(const EActInputFlag& InputFlag,
	bool bLogNotFound) const
{
	for (const FActInputFlagAction& InputFlagAction : NativeInputFlagActions)
	{
		if (InputFlagAction.InputFlag == InputFlag)
		{
			return InputFlagAction.InputAction;
		}
	}

#if WITH_EDITOR
	if (bLogNotFound)
	{
		if (const UEnum* EnumObject = StaticEnum<EActInputFlag>())
		{
			UE_LOG(LogTemp, Error, TEXT("Can't find NativeInputAction for InputFlag [%s] on InputConfig [%s]."),
				*EnumObject->GetNameStringByValue((int64)InputFlag), *GetNameSafe(this));
		}
	}
#endif
	
	return nullptr;
}
