// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralSpline.h"

// Sets default values
AProceduralSpline::AProceduralSpline()
{
	

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AProceduralSpline::BeginPlay()
{
	Super::BeginPlay();
	
}



void AProceduralSpline::PostConstruction()
{
	MainSpline->SetRelativeScale3D(Scaler);
	SetActorScale3D(FVector::OneVector);
}

// Called every frame
void AProceduralSpline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float AProceduralSpline::GetCorrectSizeForSegment(UStaticMesh* SM, ESplineMeshAxis::Type AxisType)
{
	float size = 0.0f;
	switch (AxisType)
	{
	case ESplineMeshAxis::Type::X:
		size = ((SM->GetBoundingBox().Min.GetAbs().X) + (SM->GetBoundingBox().Max.GetAbs().X))  ;
		break;
	case ESplineMeshAxis::Type::Y:
		size = (SM->GetBoundingBox().Min.GetAbs().Y) + (SM->GetBoundingBox().Max.GetAbs().Y) ;
		break;
	case ESplineMeshAxis::Type::Z:
		size = (SM->GetBoundingBox().Min.GetAbs().Z) + (SM->GetBoundingBox().Max.GetAbs().Z);
		break;
	}

	return size;
}

float AProceduralSpline::GetScaleFromPercentage(float Percentage)
{
	return 1 + (Percentage / 100);
}

float AProceduralSpline::GetScaleFromPercentageInt(int Percentage)
{
	return 1 + ((float)Percentage / 100);
}

