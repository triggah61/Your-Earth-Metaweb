// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Components/SplineMeshComponent.h"
#include "UniversalFunctionLibrary.generated.h"


/**
 * 
 */
UCLASS()
class GOTHICPACK_API UUniversalFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	//Math
		static float NormalizeValue(float Value, float minClamp, float maxClamp, float minValue, float maxValue);
		static float DivisionRemainder(float divided,float divisor);
		static int DivisionReturnValue(float divided,float divisor);
		static float ClampMoreThanZero(float Value);




		static USplineMeshComponent* AddSplineMeshComponent(UObject* outerObject, USceneComponent* attachComponent,const FVector& _RelativeLocation = FVector::ZeroVector
			, const FRotator& _RelativeRotation = FRotator::ZeroRotator, const FVector& _RelativeScale = FVector::OneVector,bool attachNow=true);

};
