// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralSpline.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Actor.h"
#include "Sidewalk.generated.h"


UENUM(BlueprintType)
enum class SidewalkSize : uint8{

	WIDE	UMETA(DisplayName = "WIDE"),
	NARROW	 UMETA(DisplayName = "NARROW")
};

USTRUCT(BlueprintType)
struct GOTHICPACK_API FSidewalkVariationParent
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "1.General Settings")
		TArray<UStaticMesh*>MiddleSidewalkMeshes;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "1.General Settings")
		UStaticMesh* EndSidewalkMesh;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "1.General Settings")
		UStaticMesh* StartSidewalkMesh;
};




UCLASS(BlueprintType)
class GOTHICPACK_API ASidewalk : public AProceduralSpline
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASidewalk();

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "1.General Settings")
		TArray<FSidewalkVariationParent> SidewalkVariation_Wide;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "1.General Settings")
		TArray<FSidewalkVariationParent> SidewalkVariation_Narrow;


	

	//Road Variables
	UPROPERTY(BlueprintReadWrite, Category = "2.Sidewalk",
		meta = (ClampMin = "0", ClampMax = "1"))
		int SidewalkTypeSelection;

	UPROPERTY(BlueprintReadWrite, Category = "2.Sidewalk")
		TArray<UStaticMesh*> Selected_MiddleSidewalk;
	UPROPERTY(BlueprintReadWrite, Category = "2.Sidewalk")
		UStaticMesh* Selected_StartSidewalk;
	UPROPERTY(BlueprintReadWrite, Category = "2.Sidewalk")
		UStaticMesh* Selected_EndSidewalk;


	UPROPERTY(BlueprintReadWrite, Category = "2.Sidewalk")
		int SidewalkMiddlePartMaxCount=0;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "2.Sidewalk")
		SidewalkSize SidewalkSizeSelection = SidewalkSize::WIDE;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "2.Sidewalk")
		bool bAddStartSection;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "2.Sidewalk")
		bool bAddEndSection;

private:

	void InitializeVariables();
	void StartBlock();
	void MiddleSection();
	void EndBlock();
	void GenerateSidewalk();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostConstruction() override;
	

};
