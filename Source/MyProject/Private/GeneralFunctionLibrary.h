// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GeneralFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class UGeneralFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "General Purpose Functions")
	static void DecrementFloatValueOfMap(const FVector& Location, FVector& Destination);
};
