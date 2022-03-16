// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralSpline.h"
#include "UObject/UObjectGlobals.h"
#include "Engine/EngineTypes.h"
#include "Math/UnrealMathUtility.h"
#include "Containers/Array.h"
#include "Engine.h"
#include "Kismet/KismetMathLibrary.h"
#include "Wall.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class GOTHICPACK_API AWall: public AProceduralSpline
{
	GENERATED_BODY()

public:
	AWall(const FObjectInitializer& OI);

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "1.General Settings")
		TArray<UStaticMesh*> Wall_StaticMeshes;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "1.General Settings")
		UStaticMesh* Gate_Middle_StaticMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "1.General Settings")
		UStaticMesh* Gate_End_StaticMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "1.General Settings")
		UStaticMesh* Gate_Start_StaticMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "1.General Settings")
		UStaticMesh* Pillar_StaticMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "1.General Settings",
		meta = (ClampMin = "0", ClampMax = "0.99"))
		float OffsetBeforeAttachMesh_Wall = 0.99f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "1.General Settings",
		meta = (ClampMin = "0", ClampMax = "0.99"))
		float OffsetBeforeAttachMesh_Gate = 0.99f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "1.General Settings")
		float PillarMaxScale = 40.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "1.General Settings")
		USplineComponent* TestSpline;

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "2.Start And End Blocks")
		bool bAddStartGate;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "2.Start And End Blocks")
		bool bAddStartPillar;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "2.Start And End Blocks",
		meta = (EditCondition = "bAddStartPillar", ClampMax = "45", ClampMin = "0"))
		int StartPillarScale;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "2.Start And End Blocks")
		bool bAddEndGate;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "2.Start And End Blocks")
		bool bAddEndPillar;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "2.Start And End Blocks",
		meta = (EditCondition = "bAddEndPillar", ClampMax = "45", ClampMin = "0"))
		int EndPillarScale;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "3.Middle Gates / Pillars")
		bool bAddMiddleGate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "3.Middle Gates / Pillars",
		meta = (EditCondition = "bAddMiddleGate", ClampMin = "1"))
		int MiddleGateGap = 1;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "3.Middle Gates / Pillars")
		bool bAddMiddlePillar;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "3.Middle Gates / Pillars",
		meta = (EditCondition = "bAddMiddlePillar && !bAddMiddleGate", ClampMin = "1"))
		int MiddlePillarGap = 3;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "3.Middle Gates / Pillars",
		meta = (EditCondition = "bAddMiddlePillar && !bAddMiddleGate"))
		int MiddlePillarGapOffset = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "3.Middle Gates / Pillars",
		meta = (EditCondition = "bAddMiddlePillar && bAddMiddleGate"))
		int MiddlePillarAmount = 0;




protected:
	virtual void BeginPlay() override;



public:
	virtual void Tick(float DeltaTime) override;
	virtual void PostConstruction() override;


private:
	void InitializeVariables();
	void StartBlock();
	void EndBlock();
	void MiddleSectionCalculation();
	void GenerateWall();

};
