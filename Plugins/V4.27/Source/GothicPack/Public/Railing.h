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
#include "Railing.generated.h"



USTRUCT(BlueprintType)
struct GOTHICPACK_API FRailingVariationParent
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1.General Settings")
		TArray<UStaticMesh*>Meshes;

};



UCLASS()
class GOTHICPACK_API ARailing : public AProceduralSpline
{
	GENERATED_BODY()
	

public:	
	ARailing(const FObjectInitializer& OI);

protected:

	UPROPERTY(BlueprintReadWrite, Category = "")
		UStaticMesh* Selected_Newel;
	UPROPERTY(BlueprintReadWrite, Category = "")
		UStaticMesh* Selected_Baluster;
	UPROPERTY(BlueprintReadWrite, Category = "")
		UStaticMesh* Selected_Prop_StartAndEnd;
	UPROPERTY(BlueprintReadWrite, Category = "")
		UStaticMesh* Selected_Prop_Middle;


	
	
	UPROPERTY(BlueprintReadWrite, Category = "")
		float NewelMaxScale = 40.0f;
	
	//UPROPERTY(EditAnywhere,Category = DefProps , meta=(UIMin = '0.0', UIMax = '1.0'))
public:
	

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category = "1.General Settings")
		TArray<FRailingVariationParent> PropVariation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "1.General Settings")
		TArray<FRailingVariationParent>  BalusterSectionVariation;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "1.General Settings")
		TArray<FRailingVariationParent>  NewelVariation ;




	//Rail Variables
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "2.Rail",
		meta = (ClampMin = "0", ClampMax= "1"))
		int BalusterTopSelection;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "2.Rail",
		meta = (ClampMin = "0", ClampMax = "3"))
		int BalusterMiddleSelection;

	//Start And End Newel / Props
	UPROPERTY(EditDefaultsOnly, Category = "3.Start And End Newel / Props")
		int NewelSelection;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "3.Start And End Newel / Props")
		bool bAddStartNewel;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "3.Start And End Newel / Props",
		meta = (EditCondition = "bAddStartNewel", ClampMax = "45",ClampMin = "0"))
		int StartNewelScale;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "3.Start And End Newel / Props")
		bool bAddEndNewel;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "3.Start And End Newel / Props",
		meta = (EditCondition = "bAddEndNewel", ClampMax = "45", ClampMin = "0"))
		int EndNewelScale;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "3.Start And End Newel / Props",
		meta = (EditCondition = "bAddEndNewel || bAddStartNewel"))
		bool bAddProp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "3.Start And End Newel / Props",
		meta = (EditCondition = "bAddProp", ClampMin = "0", ClampMax = "3"))
		int PropSelection;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "3.Start And End Newel / Props",
		meta = (EditCondition = "bAddProp", ClampMax = "30", ClampMin = "-30"))
		int PropScale;

//Middle
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "4.Middle Newels / Props")
		bool bAddMidNewel;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "4.Middle Newels / Props",
		meta = (EditCondition = "bAddMidNewel", ClampMin = "1"))
		int MiddleNewelGap= 1 ;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "4.Middle Newels / Props",
		meta = (EditCondition = "bAddMidNewel"))
		bool bMiddleNewel_AddProp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "4.Middle Newels / Props",
		meta = (EditCondition = "bMiddleNewel_AddProp && bAddMidNewel", ClampMin = "0", ClampMax = "3"))
		int MiddleNewel_PropSelection;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "4.Middle Newels / Props",
		meta = (EditCondition = "bMiddleNewel_AddProp && bAddMidNewel ",ClampMin="0"))
		int MiddleNewel_PropGap;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "4.Middle Newels / Props",
		meta = (EditCondition = "bMiddleNewel_AddProp && bAddMidNewel", ClampMax = "30", ClampMin = "-30"))
		int MiddleNewel_PropScale;
	
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
		void GenerateRailing();
		



		
};
