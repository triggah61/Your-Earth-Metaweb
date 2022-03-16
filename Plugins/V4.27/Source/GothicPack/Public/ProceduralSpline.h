// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "UniversalFunctionLibrary.h"
#include "ProceduralSpline.generated.h"




UCLASS(BlueprintType)
class GOTHICPACK_API AProceduralSpline : public AActor
{
	GENERATED_BODY()
	
public:	

	AProceduralSpline();

	

protected:
	UPROPERTY(BlueprintReadWrite,Category = "Components")
		USplineComponent* MainSpline;
	UPROPERTY(VisibleAnywhere, Category = "Components")
		USceneComponent* SceneRootComponent;
	
	UPROPERTY(BlueprintReadWrite, Category = "")
		float SegmentLength = 0.0f;
	UPROPERTY(BlueprintReadWrite, Category = "")
		float SectionLength = 0.0f;
	UPROPERTY(BlueprintReadWrite, Category = "")
		float SplineEndPointDistance = 0.0f;
	UPROPERTY(BlueprintReadWrite, Category = "")
		float SplineBeginOffset = 0.0f;
	UPROPERTY(BlueprintReadWrite, Category = "")
		float SplineEndOffset = 0.0f;
	UPROPERTY(BlueprintReadWrite, Category = "")
		int SectionCountMax = 0;


	//General Variables
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "1.General Settings")
		TEnumAsByte<ECollisionEnabled::Type> CollisionEnable = ECollisionEnabled::Type::NoCollision;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "1.General Settings")
		TEnumAsByte<ECollisionChannel> CollisionType = ECollisionChannel::ECC_WorldStatic;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "1.General Settings")
		FVector Scaler = FVector::OneVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "1.General Settings",
		meta = (ClampMin = "0", ClampMax = "0.99"))
		float  offsetBeforeAttachMesh=0.99f;

	//Spline
	UPROPERTY(VisibleAnywhere, Category = "Spline Mesh")
	TArray<USplineMeshComponent*> meshComponents;

	float GetCorrectSizeForSegment(UStaticMesh *SM, ESplineMeshAxis::Type AxisType = ESplineMeshAxis::Type::X);
	float GetScaleFromPercentage(float Percentage);
	float GetScaleFromPercentageInt(int Percentage);


	virtual void BeginPlay() override;

	

public:	

	UFUNCTION(BlueprintCallable,Category="Default")
		virtual void PostConstruction();

	virtual void Tick(float DeltaTime) override;

};
