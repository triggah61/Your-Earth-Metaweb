// Copyright 2021 Gothic Mega Pack made by Meshingun Studio. All Rights Reserved


#include "Wall.h"

AWall::AWall(const FObjectInitializer& OI)
{

	SceneRootComponent = OI.CreateDefaultSubobject<USceneComponent>(this, TEXT("SceneRoot"));


	SetRootComponent(SceneRootComponent);

	PrimaryActorTick.bCanEverTick = true;

}


void AWall::BeginPlay()
{
	Super::BeginPlay();

}

void AWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AWall::PostConstruction()
{

	Super::PostConstruction();
	InitializeVariables();
	EndBlock();
	StartBlock();
	MiddleSectionCalculation();
	

}

void AWall::InitializeVariables()
{
	SplineEndPointDistance = MainSpline->GetDistanceAlongSplineAtSplinePoint((MainSpline->GetNumberOfSplinePoints() - 1));
}



void AWall::StartBlock()
{
	//Default variables
	float startPosDistance = 0.0f, endPosDistance = 0.0f;
	FVector startPos=FVector::ZeroVector, startTangent = FVector::ZeroVector, endPos = FVector::ZeroVector, endTangent = FVector::ZeroVector;
	ESplineCoordinateSpace::Type localSpace = ESplineCoordinateSpace::Local;


#pragma region Pillar
	if (bAddStartPillar && IsValid(Pillar_StaticMesh))
	{
		//Constant variables
		float pillarScaledX = GetCorrectSizeForSegment(Pillar_StaticMesh) *GetScaleFromPercentageInt(StartPillarScale);
		float pillarScale = GetScaleFromPercentageInt(StartPillarScale);

		//Calculate start pos and end pos distances
		startPosDistance = 0.0f;
		endPosDistance = pillarScaledX;
		endPosDistance = FMath::Clamp(endPosDistance, 0.0f, SplineEndPointDistance - SplineEndOffset);

		SplineBeginOffset = pillarScaledX;
		
		//Only spawn when length of spline have reached to a point which this mesh can spawn 
		if ((SplineEndPointDistance - startPosDistance - SplineEndOffset) < (pillarScaledX * offsetBeforeAttachMesh))return;

		//Create spline mesh
		USplineMeshComponent* BeginPillarSplineMesh = UUniversalFunctionLibrary::AddSplineMeshComponent(this, RootComponent, FVector::ZeroVector, FRotator::ZeroRotator, Scaler);
		meshComponents.Add(BeginPillarSplineMesh);

		//Calculate scale based on scale variable
		FVector2D pillarScale2D = FVector2D(0.0f,0.0f);
		pillarScale2D.X = pillarScale;
		pillarScale2D.Y = UUniversalFunctionLibrary::NormalizeValue(pillarScale,
			1.0f, GetScaleFromPercentage(PillarMaxScale), 0.0f, 1.0f);
		pillarScale2D.Y = ((pillarScale2D.Y * (pillarScale - 1.0f)) * 0.3f) + 1.0f;

		//Setup required variables
		BeginPillarSplineMesh->SetStaticMesh(Pillar_StaticMesh);
		BeginPillarSplineMesh->SetForwardAxis(ESplineMeshAxis::X, false);
		BeginPillarSplineMesh->SetCollisionEnabled(CollisionEnable);
		BeginPillarSplineMesh->SetCollisionObjectType(CollisionType);
		BeginPillarSplineMesh->SetStartScale(pillarScale2D);
		BeginPillarSplineMesh->SetEndScale(pillarScale2D);

		//Calculate start pos, end pos and tangent 
		startPos = MainSpline->GetLocationAtDistanceAlongSpline(startPosDistance, localSpace);
		startTangent = MainSpline->GetTangentAtDistanceAlongSpline(startPosDistance, localSpace);
		startTangent = UKismetMathLibrary::ClampVectorSize(startTangent, SplineBeginOffset, SplineBeginOffset);


		endPos = MainSpline->GetLocationAtDistanceAlongSpline(endPosDistance, localSpace);
		endTangent = MainSpline->GetTangentAtDistanceAlongSpline(endPosDistance, localSpace);
		endTangent = UKismetMathLibrary::ClampVectorSize(endTangent, SplineBeginOffset, SplineBeginOffset);


		BeginPillarSplineMesh->SetStartAndEnd(startPos, startTangent, endPos, endTangent, true);
	}

	
#pragma endregion

#pragma region Gate
	if (bAddStartGate && IsValid(Gate_Start_StaticMesh))
	{
		//Constant variables
		float gateX = GetCorrectSizeForSegment(Gate_Start_StaticMesh);

		//Calculate start pos and end pos distances
		startPosDistance = SplineBeginOffset ;
		endPosDistance = startPosDistance + gateX;
		endPosDistance = FMath::Clamp(endPosDistance, 0.0f, SplineEndPointDistance - SplineEndOffset);

		startPosDistance = FMath::Abs(startPosDistance);


		SplineBeginOffset = endPosDistance;

		//Only spawn when length of spline have reached to a point which this mesh can spawn 
		if ((SplineEndPointDistance - startPosDistance-SplineEndOffset) <= (gateX * OffsetBeforeAttachMesh_Gate))return;
	
		//Create spline mesh
		USplineMeshComponent* BeginGateSplineMesh = UUniversalFunctionLibrary::AddSplineMeshComponent(this, RootComponent, FVector::ZeroVector, FRotator::ZeroRotator, Scaler);
		meshComponents.Add(BeginGateSplineMesh);

		FVector2D gateScale2D=FVector2D(1.0f, 1.0f);


		//Setup required variables
		BeginGateSplineMesh->SetStaticMesh(Gate_Start_StaticMesh);
		BeginGateSplineMesh->SetForwardAxis(ESplineMeshAxis::X, false);
		BeginGateSplineMesh->SetCollisionEnabled(CollisionEnable);
		BeginGateSplineMesh->SetCollisionObjectType(CollisionType);
		BeginGateSplineMesh->SetStartScale(gateScale2D);
		BeginGateSplineMesh->SetEndScale(gateScale2D);

		//Calculate start pos, end pos and tangent 
		startPos = MainSpline->GetLocationAtDistanceAlongSpline(startPosDistance, localSpace);
		startTangent = MainSpline->GetTangentAtDistanceAlongSpline(startPosDistance, localSpace);
		startTangent = UKismetMathLibrary::ClampVectorSize(startTangent, gateX, gateX);


		endPos = MainSpline->GetLocationAtDistanceAlongSpline(endPosDistance, localSpace);
		endTangent = MainSpline->GetTangentAtDistanceAlongSpline(endPosDistance, localSpace);
		endTangent = UKismetMathLibrary::ClampVectorSize(endTangent, gateX, gateX);


		BeginGateSplineMesh->SetStartAndEnd(startPos, startTangent, endPos, endTangent, true);
	}

	
#pragma endregion



}

void AWall::EndBlock()
{
	//Default variables
	FVector startPos=FVector::ZeroVector, startTangent = FVector::ZeroVector, endPos = FVector::ZeroVector, endTangent = FVector::ZeroVector;
	ESplineCoordinateSpace::Type localSpace = ESplineCoordinateSpace::Local;
	float startPosDistance = 0.0f, endPosDistance = 0.0f;

#pragma region Pillar
	if (bAddEndPillar && IsValid(Pillar_StaticMesh))
	{
		//Constant variables
		float pillarScaledX = GetCorrectSizeForSegment(Pillar_StaticMesh) *GetScaleFromPercentageInt(EndPillarScale);
		float pillarScale = GetScaleFromPercentageInt(EndPillarScale);

		SplineEndOffset = pillarScaledX;

		FVector2D pillarScale2D=FVector2D(0.0f,0.0f);
		pillarScale2D.X = pillarScale;
		pillarScale2D.Y = UUniversalFunctionLibrary::NormalizeValue(pillarScale,
			1.0f, GetScaleFromPercentage(PillarMaxScale), 0.0f, 1.0f);
		pillarScale2D.Y = ((pillarScale2D.Y * (pillarScale - 1.0f)) * 0.3f) + 1.0f;
	

		startPosDistance = SplineEndPointDistance - pillarScaledX - (25.0f *pillarScale);
		endPosDistance = SplineEndPointDistance;

		startPosDistance = FMath::Abs(startPosDistance);
		endPosDistance = FMath::Abs(endPosDistance);


		//Only spawn when length of spline have reached to a point which this mesh can spawn 
		if ((SplineEndPointDistance - startPosDistance) <= pillarScaledX * offsetBeforeAttachMesh)return;

		//Create spline mesh
		USplineMeshComponent* EndPillarSplineMesh = UUniversalFunctionLibrary::AddSplineMeshComponent(this, RootComponent, FVector::ZeroVector, FRotator::ZeroRotator, Scaler);
		meshComponents.Add(EndPillarSplineMesh);

		//Setup required variables
		EndPillarSplineMesh->SetStaticMesh(Pillar_StaticMesh);
		EndPillarSplineMesh->SetForwardAxis(ESplineMeshAxis::X, false);
		EndPillarSplineMesh->SetCollisionEnabled(CollisionEnable);
		EndPillarSplineMesh->SetCollisionObjectType(CollisionType);
		EndPillarSplineMesh->SetStartScale(pillarScale2D);
		EndPillarSplineMesh->SetEndScale(pillarScale2D);

		//Calculate tangent clamp, start position and end pos;
		startPos = MainSpline->GetLocationAtDistanceAlongSpline(startPosDistance, localSpace);
		startTangent = MainSpline->GetTangentAtDistanceAlongSpline(startPosDistance, localSpace);
		startTangent = UKismetMathLibrary::ClampVectorSize(startTangent, pillarScaledX, pillarScaledX);

		endPos = MainSpline->GetLocationAtDistanceAlongSpline(endPosDistance, localSpace);
		endTangent = MainSpline->GetTangentAtDistanceAlongSpline(endPosDistance, localSpace);
		endTangent = UKismetMathLibrary::ClampVectorSize(endTangent, pillarScaledX, pillarScaledX);

		EndPillarSplineMesh->SetStartAndEnd(startPos, startTangent, endPos, endTangent, true);
	}


#pragma endregion
#pragma region Gate
	if (bAddEndGate && IsValid(Gate_End_StaticMesh))
	{

		float gateX = GetCorrectSizeForSegment(Gate_End_StaticMesh);

		startPosDistance = SplineEndPointDistance - SplineEndOffset - gateX;
		endPosDistance = SplineEndPointDistance - SplineEndOffset;

		startPosDistance = FMath::Abs(startPosDistance);
		endPosDistance = FMath::Abs(endPosDistance);

		SplineEndOffset = SplineEndOffset + gateX;

		FVector2D gateScale2D=FVector2D(1.0f, 1.0f);

		//Only spawn when length of spline have reached to a point which this mesh can spawn 
		if ((SplineEndPointDistance - startPosDistance) <= gateX * OffsetBeforeAttachMesh_Gate)return;
		

		//Create spline mesh
		USplineMeshComponent* EndGateSplineMesh = UUniversalFunctionLibrary::AddSplineMeshComponent(this, RootComponent, FVector::ZeroVector, FRotator::ZeroRotator, Scaler);
		meshComponents.Add(EndGateSplineMesh);

		//Setup required variables
		EndGateSplineMesh->SetStaticMesh(Gate_End_StaticMesh);
		EndGateSplineMesh->SetForwardAxis(ESplineMeshAxis::X, false);
		EndGateSplineMesh->SetCollisionEnabled(CollisionEnable);
		EndGateSplineMesh->SetCollisionObjectType(CollisionType);
		EndGateSplineMesh->SetStartScale(gateScale2D);
		EndGateSplineMesh->SetEndScale(gateScale2D);
		
		//Calculate tangent clamp, start position and end pos;
		startPos = MainSpline->GetLocationAtDistanceAlongSpline(startPosDistance, localSpace);
		startTangent = MainSpline->GetTangentAtDistanceAlongSpline(startPosDistance, localSpace);
		startTangent = UKismetMathLibrary::ClampVectorSize(startTangent, gateX, gateX);

		endPos = MainSpline->GetLocationAtDistanceAlongSpline(endPosDistance, localSpace);
		endTangent = MainSpline->GetTangentAtDistanceAlongSpline(endPosDistance, localSpace);
		endTangent = UKismetMathLibrary::ClampVectorSize(endTangent, gateX, gateX);

		EndGateSplineMesh->SetStartAndEnd(startPos, startTangent, endPos, endTangent, true);
	}
#pragma endregion

}

void AWall::MiddleSectionCalculation()
{
	if (!IsValid(Gate_Middle_StaticMesh)) return;
	bool allWallsAreValid = true;
	if (Wall_StaticMeshes.Num() == 0)return;
	for (int i = 0; i < Wall_StaticMeshes.Num(); i++)
	{
		if (!IsValid(Wall_StaticMeshes[i]))allWallsAreValid = false;
	}
	if (!allWallsAreValid)return;

	SegmentLength = GetCorrectSizeForSegment(Wall_StaticMeshes[0]);
	SectionLength = UKismetMathLibrary::SelectFloat(GetCorrectSizeForSegment(Gate_Middle_StaticMesh), 0.0f, bAddMiddleGate) + (MiddleGateGap * SegmentLength);
	float sectionCountMaxFloat = MainSpline->GetSplineLength() - SplineBeginOffset - SplineEndOffset;
	sectionCountMaxFloat = UUniversalFunctionLibrary::ClampMoreThanZero(sectionCountMaxFloat);
	SectionCountMax = UUniversalFunctionLibrary::DivisionReturnValue(sectionCountMaxFloat, SectionLength);
	if (!bAddMiddleGate)SectionCountMax = 0;

	GenerateWall();
}

void AWall::GenerateWall()
{
	//Repeating Constant Variables
	float gateX = GetCorrectSizeForSegment(Gate_Middle_StaticMesh, ESplineMeshAxis::X);
	


	//Repeating Changable Variables
	int wallIndex = 0.0f;
	float correctSegmentLength = 0.0f;
	bool canAddMidGate = false;
	float tangentClamp = 0.0f;
	float startPosDistance = 0.0f, endPosDistance = 0.0f, tangentMaxClamp = 0.0f;
	float correctOffsetBeforeAttachMesh = 0.0f;
	FVector startPos=FVector::ZeroVector, startTangent = FVector::ZeroVector, endPos = FVector::ZeroVector, endTangent = FVector::ZeroVector;
	FVector2D scale2D = FVector2D(1.0f, 1.0f);
	ESplineCoordinateSpace::Type localSpace = ESplineCoordinateSpace::Local;

	//Section Loop
	for (int sectionIndex = 0; sectionIndex <= SectionCountMax; sectionIndex++)
	{
		//Calculate segment count max before starting to generate walls and gates for each section
		float segmentCountMaxF = SplineBeginOffset + (sectionIndex * SectionLength);
		segmentCountMaxF = (SplineEndPointDistance - SplineEndOffset) - segmentCountMaxF;
		int segmentCountMaxI = UUniversalFunctionLibrary::DivisionReturnValue(segmentCountMaxF, SegmentLength);
		if (bAddMiddleGate)segmentCountMaxI = FMath::Clamp(segmentCountMaxI, 0, MiddleGateGap);

		//Segment Loop
		for (int segmentIndex = 0; segmentIndex <= segmentCountMaxI; segmentIndex++)
		{
			//Begin generating wall
			correctSegmentLength = SegmentLength;
			correctOffsetBeforeAttachMesh = OffsetBeforeAttachMesh_Wall;

			canAddMidGate = false;

			//Check to see if can add mid gate
			if (segmentIndex == MiddleGateGap && bAddMiddleGate)
			{
				canAddMidGate = true;
				correctSegmentLength = GetCorrectSizeForSegment(Gate_Middle_StaticMesh);
				correctOffsetBeforeAttachMesh = OffsetBeforeAttachMesh_Gate;
			}		
			
#pragma region Wall Section

			//Calculating start position distance for wall/gate
			startPosDistance = (SegmentLength * segmentIndex) +
				SplineBeginOffset +
				SectionLength * sectionIndex;

			startPosDistance = FMath::Clamp(startPosDistance, SplineBeginOffset + (sectionIndex * SectionLength), SplineEndPointDistance - SplineEndOffset);

			//Calculating end position distance for wall/gate
			endPosDistance = SectionLength * sectionIndex +
				UKismetMathLibrary::SelectFloat((SegmentLength * segmentIndex) + correctSegmentLength, SegmentLength * (segmentIndex + 1), canAddMidGate) +
				SplineBeginOffset;

			endPosDistance = FMath::Clamp(endPosDistance, SplineBeginOffset + (sectionIndex * SectionLength), SplineEndPointDistance - SplineEndOffset);


			//Scale
			scale2D = FVector2D(1.0f, 1.0f);
	

			//Only spawn when length of spline have reached to a point which this mesh can spawn 
			if ((endPosDistance - startPosDistance) > correctSegmentLength * correctOffsetBeforeAttachMesh)
			{
				//Create SplineMeshComponent
				USplineMeshComponent* WallSplineMesh = UUniversalFunctionLibrary::AddSplineMeshComponent(this, RootComponent, FVector::ZeroVector, FRotator::ZeroRotator, Scaler);
				meshComponents.Add(WallSplineMesh);

				//Choose between wall or gate static mesh at the end of each loop
				UStaticMesh* wallOrGateSM = canAddMidGate ? Gate_Middle_StaticMesh: Wall_StaticMeshes[wallIndex];
				
				//Setup required variables
				WallSplineMesh->SetStaticMesh(wallOrGateSM);
				WallSplineMesh->SetForwardAxis(ESplineMeshAxis::X, false);
				WallSplineMesh->SetCollisionEnabled(CollisionEnable);
				WallSplineMesh->SetCollisionObjectType(CollisionType);
				WallSplineMesh->SetStartScale(scale2D);
				WallSplineMesh->SetEndScale(scale2D);

				//Calculate tangent clamp, start position and end pos;
				tangentClamp = correctSegmentLength;;

				startPos = MainSpline->GetLocationAtDistanceAlongSpline(startPosDistance, localSpace);
				startTangent = MainSpline->GetTangentAtDistanceAlongSpline(startPosDistance, localSpace);
				startTangent = UKismetMathLibrary::ClampVectorSize(startTangent, tangentClamp, tangentClamp);

				endPos = MainSpline->GetLocationAtDistanceAlongSpline(endPosDistance, localSpace);
				endTangent = MainSpline->GetTangentAtDistanceAlongSpline(endPosDistance, localSpace);
				endTangent = UKismetMathLibrary::ClampVectorSize(endTangent, tangentClamp, tangentClamp);

				WallSplineMesh->SetStartAndEnd(startPos, startTangent, endPos, endTangent, true);
				//Increase wall mesh index
				wallIndex++;

				//Reset wall index if it reaches array's end
				if (wallIndex == Wall_StaticMeshes.Num())
				{
					wallIndex = 0;
				}
			}

			
			
#pragma endregion



#pragma region MidPillar
			if (!bAddMiddlePillar || !IsValid(Pillar_StaticMesh))continue;

			float pillarX = GetCorrectSizeForSegment(Pillar_StaticMesh);


			if (!bAddMiddleGate)
			{
				
				int index = segmentIndex;
				//Conditions

				if (segmentIndex == segmentCountMaxI && sectionIndex == SectionCountMax && (bAddEndGate || bAddEndPillar))continue;
				if (segmentIndex == 0 && sectionIndex == 0 && (bAddStartGate || bAddStartPillar))continue;


				index += MiddlePillarGapOffset;

				if (UUniversalFunctionLibrary::DivisionRemainder((float)index, MiddlePillarGap) != 0) continue;

				

				//Calculating start position distance for mid-pillar
				startPosDistance = (SegmentLength * segmentIndex) +
					SplineBeginOffset +
					SectionLength * sectionIndex;


				startPosDistance = FMath::Clamp(startPosDistance, SplineBeginOffset + (sectionIndex * SectionLength), SplineEndPointDistance - SplineEndOffset);

				//Calculating end position distance for mid-pillar
				endPosDistance = SectionLength * sectionIndex +
			(SegmentLength * segmentIndex) + pillarX +
					SplineBeginOffset;

				endPosDistance = FMath::Clamp(endPosDistance, SplineBeginOffset + (sectionIndex * SectionLength), SplineEndPointDistance - SplineEndOffset);


				if ((endPosDistance - startPosDistance) < pillarX * correctOffsetBeforeAttachMesh)continue;

				//Create SplineMeshComponent
				USplineMeshComponent* PillarSplineMesh = UUniversalFunctionLibrary::AddSplineMeshComponent(this, RootComponent, FVector::ZeroVector, FRotator::ZeroRotator, Scaler);
				meshComponents.Add(PillarSplineMesh);

				//Setup required varaibles
				PillarSplineMesh->SetStaticMesh(Pillar_StaticMesh);
				PillarSplineMesh->SetForwardAxis(ESplineMeshAxis::X, false);
				PillarSplineMesh->SetCollisionEnabled(CollisionEnable);
				PillarSplineMesh->SetCollisionObjectType(CollisionType);
				PillarSplineMesh->SetStartScale(scale2D);
				PillarSplineMesh->SetEndScale(scale2D);


				//Calculate tangent clamp, start position and end pos;
				tangentClamp = pillarX;;

				startPos = MainSpline->GetLocationAtDistanceAlongSpline(startPosDistance, localSpace);
				startTangent = MainSpline->GetTangentAtDistanceAlongSpline(startPosDistance, localSpace);
				startTangent = UKismetMathLibrary::ClampVectorSize(startTangent, tangentClamp, tangentClamp);


				endPos = MainSpline->GetLocationAtDistanceAlongSpline(endPosDistance, localSpace);
				endTangent = MainSpline->GetTangentAtDistanceAlongSpline(endPosDistance, localSpace);
				endTangent = UKismetMathLibrary::ClampVectorSize(endTangent, tangentClamp, tangentClamp);

				PillarSplineMesh->SetStartAndEnd(startPos, startTangent, endPos, endTangent, true);
			}
			else 
			{
				if (segmentIndex == segmentCountMaxI)
				{
					//Get begin of the section in spline length
					float beginOfSection =
						SplineBeginOffset +
						SectionLength * sectionIndex;

					//Get end of the section in spline length
					float endOfSection= (SegmentLength * MiddleGateGap-1) +
						SplineBeginOffset +
						SectionLength * sectionIndex;

					//Calculate how many pillar can fit inside properly without exceeding section length
					float lengthOfSection = endOfSection - beginOfSection;
					if (lengthOfSection <=0) continue;
					int numberOfEligiblePillar = MiddlePillarAmount;
					float lengthLeft = lengthOfSection - (numberOfEligiblePillar * pillarX);
					
					while (lengthLeft< 0)
					{
						numberOfEligiblePillar--;
						numberOfEligiblePillar = FMath::Clamp(numberOfEligiblePillar, 0, MiddlePillarAmount);
						lengthLeft= lengthOfSection - (numberOfEligiblePillar * pillarX);
						
					}
					
					//Distance between each pillar
					float distanceBetweenEachPillar = (lengthLeft / (numberOfEligiblePillar+1));

					//Loop to create pillars
					for (int i = 0; i < numberOfEligiblePillar; i++)
					{
						//Calculating start position distance for mid-pillar
						startPosDistance = 
							SplineBeginOffset +
							(SectionLength * sectionIndex)+ ((i + 1)*distanceBetweenEachPillar) + (i*pillarX);


						startPosDistance = FMath::Clamp(startPosDistance, SplineBeginOffset + (sectionIndex * SectionLength), SplineEndPointDistance - SplineEndOffset);

						//Calculating end position distance for mid-pillar
						endPosDistance = startPosDistance+ pillarX;
							

						endPosDistance = FMath::Clamp(endPosDistance, SplineBeginOffset + (sectionIndex * SectionLength), SplineEndPointDistance-SplineEndOffset);

						//Scale
						scale2D = FVector2D(1.0f, 1.0f);

						//Only spawn when length of spline have reached to a point which this mesh can spawn 
						if ((endPosDistance - startPosDistance) >= pillarX * OffsetBeforeAttachMesh_Gate)
						{
							//Generate splinemesh
							USplineMeshComponent* PillarSplineMesh = UUniversalFunctionLibrary::AddSplineMeshComponent(this, RootComponent, FVector::ZeroVector, FRotator::ZeroRotator, Scaler);
							meshComponents.Add(PillarSplineMesh);

							//Setup required variables
							PillarSplineMesh->SetStaticMesh(Pillar_StaticMesh);
							PillarSplineMesh->SetForwardAxis(ESplineMeshAxis::X, false);
							PillarSplineMesh->SetCollisionEnabled(CollisionEnable);
							PillarSplineMesh->SetCollisionObjectType(CollisionType);
							PillarSplineMesh->SetStartScale(scale2D);
							PillarSplineMesh->SetEndScale(scale2D);


							//Calculate tangent clamp, start position and end pos;
							tangentClamp = pillarX;;

							startPos = MainSpline->GetLocationAtDistanceAlongSpline(startPosDistance, localSpace);
							startTangent = MainSpline->GetTangentAtDistanceAlongSpline(startPosDistance, localSpace);
							startTangent = UKismetMathLibrary::ClampVectorSize(startTangent, tangentClamp, tangentClamp);


							endPos = MainSpline->GetLocationAtDistanceAlongSpline(endPosDistance, localSpace);
							endTangent = MainSpline->GetTangentAtDistanceAlongSpline(endPosDistance, localSpace);
							endTangent = UKismetMathLibrary::ClampVectorSize(endTangent, tangentClamp, tangentClamp);

							PillarSplineMesh->SetStartAndEnd(startPos, startTangent, endPos, endTangent, true);
						}



						
					}
				}
			}
			

#pragma endregion

		}

	}


}


