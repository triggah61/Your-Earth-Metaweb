// Fill out your copyright notice in the Description page of Project Settings.


#include "UniversalFunctionLibrary.h"

float UUniversalFunctionLibrary::NormalizeValue(float Value, float minClamp, float maxClamp, float minValue, float maxValue)
{
	
	float result=0.0f;
	Value = FMath::Clamp(Value,minClamp, maxClamp);
	result = (Value - minClamp) / (maxClamp - minClamp);
	result = result * (maxValue - minValue);
	result = result + minValue;
	result = FMath::Abs(result);
	return result;
}

float UUniversalFunctionLibrary::DivisionRemainder(float divided, float divisor)
{
	float result = 0.0f;
	if (divisor == 0.0f) return 0.0f;
	else
	{
		result = FMath::Fmod(divided, divisor);
		return result;
	}
}

int UUniversalFunctionLibrary::DivisionReturnValue(float divided, float divisor)
{
	int result = 0;
	if (divisor == 0.0f) return 0;
	else
	{
		result = (int)divided / divisor;
		return result;
	}
}

float UUniversalFunctionLibrary::ClampMoreThanZero(float Value)
{

	float result = Value;
	result = FMath::Clamp(result, 0.0f, FMath::Abs(result));
	return result;
}


USplineMeshComponent* UUniversalFunctionLibrary::AddSplineMeshComponent(UObject* outerObject, USceneComponent* attachComponent, const FVector& _RelativeLocation, const FRotator& _RelativeRotation, const FVector& _RelativeScale, bool attachNow)
{
	USplineMeshComponent *splineMesh = NewObject<USplineMeshComponent>(outerObject, USplineMeshComponent::StaticClass());

		splineMesh->SetRelativeLocation(_RelativeLocation);
		splineMesh->SetRelativeRotation(_RelativeRotation);
		splineMesh->SetRelativeScale3D(_RelativeScale);
		splineMesh->RegisterComponent();
		splineMesh->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		splineMesh->AttachToComponent(attachComponent,
			FAttachmentTransformRules::FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
		
	
	return splineMesh;
}
