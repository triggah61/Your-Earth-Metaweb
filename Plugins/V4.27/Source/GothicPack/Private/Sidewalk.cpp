// Copyright 2021 Gothic Mega Pack made by Meshingun Studio. All Rights Reserved


#include "Sidewalk.h"


ASidewalk::ASidewalk()
{
	SceneRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	PrimaryActorTick.bCanEverTick = true;

}


void ASidewalk::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASidewalk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASidewalk::PostConstruction()
{
	Super::PostConstruction();
	InitializeVariables();
	StartBlock();
	EndBlock();
	MiddleSection();
	GenerateSidewalk();
}

void ASidewalk::InitializeVariables()
{
	SplineEndPointDistance = MainSpline->GetDistanceAlongSplineAtSplinePoint((MainSpline->GetNumberOfSplinePoints() - 1));
}

void ASidewalk::StartBlock()
{
	if (!bAddStartSection) return;

	float startPosDistance = 0.0f, endPosDistance = 0.0f;
	FVector startPos=FVector::ZeroVector, endPos = FVector::ZeroVector,startTangent = FVector::ZeroVector,endTangent = FVector::ZeroVector;
	ESplineCoordinateSpace::Type localSpace = ESplineCoordinateSpace::Local;


	//Get the start block length (X axis) and set the spline begin offset to it
	float sidewalkLength = GetCorrectSizeForSegment(Selected_StartSidewalk);
	SplineBeginOffset = sidewalkLength;

	USplineMeshComponent* BeginSidewalkSplineMesh = UUniversalFunctionLibrary::AddSplineMeshComponent(this, RootComponent, FVector::ZeroVector, FRotator::ZeroRotator, Scaler);
	meshComponents.Add(BeginSidewalkSplineMesh);

	FVector2D newelScale2D= FVector2D(0.0f,0.0f);
	
	//Select Static Mesh Settings
	BeginSidewalkSplineMesh->SetStaticMesh(Selected_StartSidewalk);
	BeginSidewalkSplineMesh->SetForwardAxis(ESplineMeshAxis::X, false);
	BeginSidewalkSplineMesh->SetCollisionEnabled(CollisionEnable);
	BeginSidewalkSplineMesh->SetCollisionObjectType(CollisionType);

	//Calculate start and end pos distances for the mesh in our spline
	startPosDistance = 0.0f;
	endPosDistance = SplineBeginOffset;


	startPos = MainSpline->GetLocationAtDistanceAlongSpline(startPosDistance, localSpace);
	startTangent = MainSpline->GetTangentAtDistanceAlongSpline(startPosDistance, localSpace);
	startTangent = UKismetMathLibrary::ClampVectorSize(startTangent, SplineBeginOffset, SplineBeginOffset);


	endPos = MainSpline->GetLocationAtDistanceAlongSpline(endPosDistance, localSpace);
	endTangent = MainSpline->GetTangentAtDistanceAlongSpline(endPosDistance, localSpace);
	endTangent = UKismetMathLibrary::ClampVectorSize(endTangent, SplineBeginOffset, SplineBeginOffset);

	BeginSidewalkSplineMesh->SetStartAndEnd(startPos, startTangent, endPos, endTangent);


}

void ASidewalk::MiddleSection()
{
	SegmentLength = GetCorrectSizeForSegment(Selected_MiddleSidewalk[0]);

	//Considering End Section Size
	
	float splineLength = SplineEndPointDistance - SplineBeginOffset -SplineEndOffset ;
	splineLength = UUniversalFunctionLibrary::ClampMoreThanZero(splineLength);
	SectionCountMax = UUniversalFunctionLibrary::DivisionReturnValue(splineLength, SegmentLength);


	
}

void ASidewalk::EndBlock()
{
	if (!bAddEndSection) return;

	float startPosDistance = 0.0f, endPosDistance = 0.0f;
	FVector startPos = FVector::ZeroVector, endPos = FVector::ZeroVector, startTangent = FVector::ZeroVector, endTangent = FVector::ZeroVector;
	ESplineCoordinateSpace::Type localSpace = ESplineCoordinateSpace::Local;


	//Get the start block length (X axis) and set the spline begin offset to it
	float sidewalkLength = GetCorrectSizeForSegment(Selected_EndSidewalk);
	SplineEndOffset = sidewalkLength;

	USplineMeshComponent* EndSidewalkSplineMesh = UUniversalFunctionLibrary::AddSplineMeshComponent(this, RootComponent, FVector::ZeroVector, FRotator::ZeroRotator, Scaler);
	meshComponents.Add(EndSidewalkSplineMesh);

	FVector2D newelScale2D= FVector2D(0.0f, 0.0f);

	//Select Static Mesh Settings
	EndSidewalkSplineMesh->SetStaticMesh(Selected_EndSidewalk);
	EndSidewalkSplineMesh->SetForwardAxis(ESplineMeshAxis::X, false);
	EndSidewalkSplineMesh->SetCollisionEnabled(CollisionEnable);
	EndSidewalkSplineMesh->SetCollisionObjectType(CollisionType);

	//Calculate start and end pos distances for the mesh in our spline
	startPosDistance = SplineEndPointDistance-SplineEndOffset;
	startPosDistance = FMath::Clamp(startPosDistance, SplineBeginOffset, startPosDistance);

	endPosDistance = SplineEndPointDistance;


	startPos = MainSpline->GetLocationAtDistanceAlongSpline(startPosDistance, localSpace);
	startTangent = MainSpline->GetTangentAtDistanceAlongSpline(startPosDistance, localSpace);
	startTangent = UKismetMathLibrary::ClampVectorSize(startTangent, SplineEndOffset, SplineEndOffset);


	endPos = MainSpline->GetLocationAtDistanceAlongSpline(endPosDistance, localSpace);
	endTangent = MainSpline->GetTangentAtDistanceAlongSpline(endPosDistance, localSpace);
	endTangent = UKismetMathLibrary::ClampVectorSize(endTangent, SplineEndOffset, SplineEndOffset);

	EndSidewalkSplineMesh->SetStartAndEnd(startPos, startTangent, endPos, endTangent);
}

void ASidewalk::GenerateSidewalk()
{
	float startPosDistance = 0.0f, endPosDistance = 0.0f, segmentLength = 0.0f;
	int middleBlockIndex = 0;
	FVector startPos = FVector::ZeroVector, endPos = FVector::ZeroVector, startTangent = FVector::ZeroVector, endTangent = FVector::ZeroVector;
	ESplineCoordinateSpace::Type localSpace = ESplineCoordinateSpace::Local;
	
	for (int segmentIndex = 0; segmentIndex <= SectionCountMax; segmentIndex++)
	{
		if (middleBlockIndex == SidewalkMiddlePartMaxCount )middleBlockIndex = 0;
		segmentLength = GetCorrectSizeForSegment(Selected_MiddleSidewalk[middleBlockIndex]);


		float tangentClamp = FMath::Clamp(segmentLength, 0.0f, SplineEndPointDistance - SplineEndOffset);
		startPosDistance = SplineBeginOffset + (segmentIndex * segmentLength);

		startPosDistance = FMath::Clamp(startPosDistance, SplineBeginOffset + (segmentIndex * SegmentLength), SplineEndPointDistance - SplineEndOffset);


		endPosDistance = SplineBeginOffset + ((segmentIndex + 1) * segmentLength);

		endPosDistance = FMath::Clamp(endPosDistance, SplineBeginOffset + (segmentIndex * SegmentLength), SplineEndPointDistance - SplineEndOffset);

		bool attach = false;
		if ((endPosDistance - startPosDistance) >= segmentLength * offsetBeforeAttachMesh)
		{
			USplineMeshComponent* MiddleSideWalkSplineMesh = UUniversalFunctionLibrary::AddSplineMeshComponent(this, RootComponent, FVector::ZeroVector, FRotator::ZeroRotator, Scaler);
			meshComponents.Add(MiddleSideWalkSplineMesh);

			MiddleSideWalkSplineMesh->SetStaticMesh(Selected_MiddleSidewalk[middleBlockIndex]);
			MiddleSideWalkSplineMesh->SetForwardAxis(ESplineMeshAxis::X, false);
			MiddleSideWalkSplineMesh->SetCollisionEnabled(CollisionEnable);
			MiddleSideWalkSplineMesh->SetCollisionObjectType(CollisionType);





			startPos = MainSpline->GetLocationAtDistanceAlongSpline(startPosDistance, localSpace);
			startTangent = MainSpline->GetTangentAtDistanceAlongSpline(startPosDistance, localSpace);
			startTangent = UKismetMathLibrary::ClampVectorSize(startTangent, tangentClamp, tangentClamp);


			endPos = MainSpline->GetLocationAtDistanceAlongSpline(endPosDistance, localSpace);
			endTangent = MainSpline->GetTangentAtDistanceAlongSpline(endPosDistance, localSpace);
			endTangent = UKismetMathLibrary::ClampVectorSize(endTangent, tangentClamp, tangentClamp);

			MiddleSideWalkSplineMesh->SetStartAndEnd(startPos, startTangent, endPos, endTangent);
			middleBlockIndex++;
		}
	
		





		///
	}

	
	
}
