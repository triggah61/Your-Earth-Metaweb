// Copyright 2021 Gothic Mega Pack made by Meshingun Studio. All Rights Reserved


#include "Railing.h"

ARailing::ARailing(const FObjectInitializer& OI) 
{	
	
	SceneRootComponent = OI.CreateDefaultSubobject<USceneComponent>(this,TEXT("SceneRoot"));
	SetRootComponent(SceneRootComponent);
	
	PrimaryActorTick.bCanEverTick = true;

}

 void ARailing::PostConstruction()
{ 
	Super::PostConstruction();
	InitializeVariables();
	StartBlock();
	EndBlock();
	MiddleSectionCalculation();
	GenerateRailing();

}

void ARailing::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARailing::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void ARailing::InitializeVariables()
{	
	SplineEndPointDistance = MainSpline->GetDistanceAlongSplineAtSplinePoint((MainSpline->GetNumberOfSplinePoints() - 1));
}
void ARailing::StartBlock()
{
	if (!bAddStartNewel || !IsValid(Selected_Newel))return;

	float newelScaledX = GetCorrectSizeForSegment(Selected_Newel) *GetScaleFromPercentageInt(StartNewelScale);
	float newelX = GetCorrectSizeForSegment(Selected_Newel);
	float newelZ = GetCorrectSizeForSegment(Selected_Newel,ESplineMeshAxis::Z);
	float newelScale = GetScaleFromPercentageInt(StartNewelScale);

	float distance = 0.0f,startPosDistance = 0.0f,endPosDistance = 0.0f;

	FVector startPos= FVector::ZeroVector, startTangent = FVector::ZeroVector, endPos = FVector::ZeroVector, endTangent = FVector::ZeroVector;
	ESplineCoordinateSpace::Type localSpace = ESplineCoordinateSpace::Local;


	//Newel At Start
	SplineBeginOffset = newelScaledX;
	USplineMeshComponent* BeginNewelSplineMesh = UUniversalFunctionLibrary::AddSplineMeshComponent(this, RootComponent,FVector::ZeroVector, FRotator::ZeroRotator, Scaler);
	meshComponents.Add(BeginNewelSplineMesh);
	
	FVector2D newelScale2D = FVector2D(0.0f,0.0f);
	newelScale2D.X = newelScale;
	newelScale2D.Y = UUniversalFunctionLibrary::NormalizeValue(newelScale,
		1.0f, GetScaleFromPercentage(NewelMaxScale), 0.0f, 1.0f);
	newelScale2D.Y = ((newelScale2D.Y * (newelScale -1.0f )) * 0.3f) + 1.0f;
	//Select Static Mesh Settings
	BeginNewelSplineMesh->SetStaticMesh(Selected_Newel);
	BeginNewelSplineMesh->SetForwardAxis(ESplineMeshAxis::X, false);
	BeginNewelSplineMesh->SetCollisionEnabled(CollisionEnable);
	BeginNewelSplineMesh->SetCollisionObjectType(CollisionType);
	BeginNewelSplineMesh->SetStartScale(newelScale2D);
	BeginNewelSplineMesh->SetEndScale(newelScale2D);
	//Set Begin and End

	if (IsValid(Selected_Prop_StartAndEnd))
	{
		distance = UKismetMathLibrary::SelectFloat(0.0f,
			((GetCorrectSizeForSegment(Selected_Prop_StartAndEnd)*GetScaleFromPercentageInt(PropScale)) - SplineBeginOffset) / 2.0f
			, !bAddProp);
	}
	
	distance = FMath::Clamp(distance, 0.0f, distance);

	startPosDistance = distance;
	startPos = MainSpline->GetLocationAtDistanceAlongSpline(startPosDistance,localSpace);
	startTangent = MainSpline->GetTangentAtDistanceAlongSpline(startPosDistance, localSpace);
	startTangent = UKismetMathLibrary::ClampVectorSize(startTangent, SplineBeginOffset, SplineBeginOffset);

	endPosDistance = distance + SplineBeginOffset;
	endPos = MainSpline->GetLocationAtDistanceAlongSpline(endPosDistance, localSpace);
	endTangent = MainSpline->GetTangentAtDistanceAlongSpline(endPosDistance, localSpace);
	endTangent = UKismetMathLibrary::ClampVectorSize(endTangent, SplineBeginOffset, SplineBeginOffset);

	
	BeginNewelSplineMesh->SetStartAndEnd(startPos, startTangent, endPos, endTangent, true);
	



	//Return the function if user dont want prop
	if (!bAddProp || !IsValid(Selected_Prop_StartAndEnd)) return;

	//Needed variables
	float propX = GetCorrectSizeForSegment(Selected_Prop_StartAndEnd);
	float propScale = GetScaleFromPercentageInt(PropScale);
	float propScaledX = GetCorrectSizeForSegment(Selected_Prop_StartAndEnd) * GetScaleFromPercentage(PropScale);
	float propAndNewelDivide = (propScaledX - SplineBeginOffset) / 2.0f;
	float absPropAndNewelDivide = FMath::Clamp(propAndNewelDivide, 0.0f, propAndNewelDivide);
	propAndNewelDivide = absPropAndNewelDivide - propAndNewelDivide;


	FVector upVector = MainSpline->GetUpVectorAtDistanceAlongSpline((SplineBeginOffset / 2.0f) + propAndNewelDivide, ESplineCoordinateSpace::Local);
	FVector propRelativeLocation = upVector * (Scaler.Z * newelScale2D.Y *newelZ);
	//Construct SplineMesh and apply settings to it
	USplineMeshComponent* PropSplineMesh = UUniversalFunctionLibrary::AddSplineMeshComponent(this, RootComponent, propRelativeLocation, FRotator::ZeroRotator, Scaler);
	meshComponents.Add(PropSplineMesh);

	FVector2D propScale2D =  FVector2D(propScale, propScale);
	PropSplineMesh->SetStaticMesh(Selected_Prop_StartAndEnd);
	PropSplineMesh->SetForwardAxis(ESplineMeshAxis::X, false);
	PropSplineMesh->SetCollisionEnabled(CollisionEnable);
	PropSplineMesh->SetCollisionObjectType(CollisionType);
	PropSplineMesh->SetStartScale(propScale2D);
	PropSplineMesh->SetEndScale(propScale2D);

	//Calculations for SplineMesh start and end location / tangent
	startPosDistance = propAndNewelDivide;
	startPos = MainSpline->GetLocationAtDistanceAlongSpline(startPosDistance,ESplineCoordinateSpace::Local);
	startTangent = MainSpline->GetTangentAtDistanceAlongSpline(startPosDistance, ESplineCoordinateSpace::Local);
	startTangent= UKismetMathLibrary::ClampVectorSize(startTangent, propScaledX, propScaledX);

	endPosDistance = propAndNewelDivide + propScaledX;
	endPos = MainSpline->GetLocationAtDistanceAlongSpline(endPosDistance, ESplineCoordinateSpace::Local);
	endTangent = MainSpline->GetTangentAtDistanceAlongSpline(endPosDistance, ESplineCoordinateSpace::Local);
	endTangent = UKismetMathLibrary::ClampVectorSize(endTangent, propScaledX, propScaledX);

	PropSplineMesh->SetStartAndEnd(startPos, startTangent, endPos, endTangent, true);
	
	SplineBeginOffset = SplineBeginOffset + distance;

}

void ARailing::EndBlock()
{
	if (!bAddEndNewel || !IsValid(Selected_Newel))return;

	float newelScaledX = GetCorrectSizeForSegment(Selected_Newel) *GetScaleFromPercentageInt(EndNewelScale);
	float newelX = GetCorrectSizeForSegment(Selected_Newel);
	float newelZ = GetCorrectSizeForSegment(Selected_Newel, ESplineMeshAxis::Z);
	float newelScale = GetScaleFromPercentageInt(EndNewelScale);

	float distance = 0.0f;
	float startPosDistance=0.0f, endPosDistance = 0.0f;
	
	FVector startPos=FVector::ZeroVector, startTangent = FVector::ZeroVector, endPos = FVector::ZeroVector, endTangent = FVector::ZeroVector;
	ESplineCoordinateSpace::Type localSpace= ESplineCoordinateSpace::Local;


	//Newel At Start
	SplineEndOffset = newelScaledX;
	USplineMeshComponent* EndNewelSplineMesh = UUniversalFunctionLibrary::AddSplineMeshComponent(this, RootComponent, FVector::ZeroVector, FRotator::ZeroRotator, Scaler);
	meshComponents.Add(EndNewelSplineMesh);
	

	FVector2D newelScale2D= FVector2D(0.0f, 0.0f);
	newelScale2D.X = newelScale;
	newelScale2D.Y = UUniversalFunctionLibrary::NormalizeValue(newelScale,
		1.0f, GetScaleFromPercentage(NewelMaxScale), 0.0f, 1.0f);
	newelScale2D.Y = ((newelScale2D.Y * (newelScale - 1.0f)) * 0.3f) + 1.0f;
	//Select Static Mesh Settings
	EndNewelSplineMesh->SetStaticMesh(Selected_Newel);
	EndNewelSplineMesh->SetForwardAxis(ESplineMeshAxis::X, false);
	EndNewelSplineMesh->SetCollisionEnabled(CollisionEnable);
	EndNewelSplineMesh->SetCollisionObjectType(CollisionType);
	EndNewelSplineMesh->SetStartScale(newelScale2D);
	EndNewelSplineMesh->SetEndScale(newelScale2D);
	//Set Begin and End
	
	if (IsValid(Selected_Prop_StartAndEnd))
	{
		distance = UKismetMathLibrary::SelectFloat(0.0f,
			((GetCorrectSizeForSegment(Selected_Prop_StartAndEnd)*GetScaleFromPercentageInt(PropScale)) - SplineEndOffset) / 2.0f
			, !bAddProp);
	}

	distance = FMath::Clamp(distance, 0.0f, distance);

	startPosDistance = (SplineEndPointDistance - SplineEndOffset) - distance;
	startPos = MainSpline->GetLocationAtDistanceAlongSpline(startPosDistance, localSpace);
	startTangent = MainSpline->GetTangentAtDistanceAlongSpline(startPosDistance, localSpace);
	startTangent = UKismetMathLibrary::ClampVectorSize(startTangent, SplineEndOffset, SplineEndOffset);

	endPosDistance = SplineEndPointDistance - distance;
	endPos = MainSpline->GetLocationAtDistanceAlongSpline(endPosDistance, localSpace);
	endTangent = MainSpline->GetTangentAtDistanceAlongSpline(endPosDistance, localSpace);
	endTangent = UKismetMathLibrary::ClampVectorSize(endTangent, SplineEndOffset, SplineEndOffset);


	EndNewelSplineMesh->SetStartAndEnd(startPos, startTangent, endPos, endTangent, true);




	//Prop
	if (!bAddProp || !IsValid(Selected_Prop_StartAndEnd)) return;
	float propX = GetCorrectSizeForSegment(Selected_Prop_StartAndEnd);
	float propScale = GetScaleFromPercentageInt(PropScale);
	float propScaledX = GetCorrectSizeForSegment(Selected_Prop_StartAndEnd) * GetScaleFromPercentage(PropScale);
	

	FVector upVector = MainSpline->GetUpVectorAtDistanceAlongSpline(SplineEndPointDistance - (SplineEndOffset / 2.0f)-distance, ESplineCoordinateSpace::Local);
	FVector propRelativeLocation = upVector * (Scaler.Z * newelScale2D.Y *newelZ);
	USplineMeshComponent* PropSplineMesh = UUniversalFunctionLibrary::AddSplineMeshComponent(this, RootComponent, propRelativeLocation, FRotator::ZeroRotator, Scaler);
	meshComponents.Add(PropSplineMesh);

	FVector2D propScale2D = FVector2D(propScale, propScale);
	PropSplineMesh->SetStaticMesh(Selected_Prop_StartAndEnd);
	PropSplineMesh->SetForwardAxis(ESplineMeshAxis::X, false);
	PropSplineMesh->SetCollisionEnabled(CollisionEnable);
	PropSplineMesh->SetCollisionObjectType(CollisionType);
	PropSplineMesh->SetStartScale(propScale2D);
	PropSplineMesh->SetEndScale(propScale2D);


	startPosDistance = SplineEndPointDistance - (SplineEndOffset / 2.0f) - (propScaledX / 2.0f) - distance;
	startPos = MainSpline->GetLocationAtDistanceAlongSpline(startPosDistance, localSpace);
	startTangent = MainSpline->GetTangentAtDistanceAlongSpline(startPosDistance, localSpace);
	startTangent = UKismetMathLibrary::ClampVectorSize(startTangent, propScaledX, propScaledX);

	endPosDistance = SplineEndPointDistance - (SplineEndOffset / 2.0f) + (propScaledX / 2.0f) + distance;
	endPos = MainSpline->GetLocationAtDistanceAlongSpline(endPosDistance, localSpace);
	endTangent = MainSpline->GetTangentAtDistanceAlongSpline(endPosDistance, localSpace);
	endTangent = UKismetMathLibrary::ClampVectorSize(endTangent, propScaledX, propScaledX);

	PropSplineMesh->SetStartAndEnd(startPos, startTangent, endPos, endTangent, true);

	SplineEndOffset = SplineEndOffset + distance;
}

void ARailing::MiddleSectionCalculation()
{
	if(IsValid(Selected_Baluster))SegmentLength = GetCorrectSizeForSegment(Selected_Baluster);
	
	
	SectionLength = UKismetMathLibrary::SelectFloat(GetCorrectSizeForSegment(Selected_Newel), 0.0f, bAddMidNewel) + (MiddleNewelGap * SegmentLength);
	float sectionCountMaxFloat = MainSpline->GetSplineLength()- SplineBeginOffset - SplineEndOffset ;
	sectionCountMaxFloat = UUniversalFunctionLibrary::ClampMoreThanZero(sectionCountMaxFloat);
	SectionCountMax = UUniversalFunctionLibrary::DivisionReturnValue(sectionCountMaxFloat, SectionLength);
	if (!bAddMidNewel)SectionCountMax = 0;
	

	
}

void ARailing::GenerateRailing()
{
	//Default Variables
	int middleNewelPropIndex = 0;
	int middleNewelFlipIndex = 0;
	float correctSegmentLength = 0;
	bool canAddMidNewel = false;

	float startPosDistance = 0.0f, endPosDistance = 0.0f,tangentMaxClamp = 0.0f;
	FVector startPos = FVector::ZeroVector, startTangent = FVector::ZeroVector, endPos = FVector::ZeroVector, endTangent=FVector::ZeroVector;
	FVector2D scale2D = FVector2D(1.0f, 1.0f);
	ESplineCoordinateSpace::Type localSpace = ESplineCoordinateSpace::Local;

	//Section Calculation
	for (int sectionIndex = 0; sectionIndex <= SectionCountMax; sectionIndex++)
	{
		//Middle newel flip index changes compared to its last index to avoid repeatition
		middleNewelFlipIndex = UKismetMathLibrary::SelectInt(1, 0, UUniversalFunctionLibrary::DivisionRemainder(middleNewelFlipIndex,2.0f)==0.0f);

		//Calculate segment count max before starting to generate rail for each section
		float segmentCountMaxF = SplineBeginOffset + (sectionIndex * SectionLength);
		segmentCountMaxF = (SplineEndPointDistance - SplineEndOffset) - segmentCountMaxF ;
		int segmentCountMaxI = UUniversalFunctionLibrary::DivisionReturnValue(segmentCountMaxF, SegmentLength);
		if(bAddMidNewel)segmentCountMaxI = FMath::Clamp(segmentCountMaxI, 0, MiddleNewelGap);

		for (int segmentIndex = 0; segmentIndex <= segmentCountMaxI; segmentIndex++)
		{
			//Begin generating railway
			//Start with props

			correctSegmentLength = SegmentLength;
			canAddMidNewel = false;

			//Check to see if can add mid newel
			if (segmentIndex == MiddleNewelGap && bAddMidNewel)
			{
				canAddMidNewel = true;
				correctSegmentLength = GetCorrectSizeForSegment(Selected_Newel);
			}

			//Repeating Variables
			float newelX = GetCorrectSizeForSegment(Selected_Newel, ESplineMeshAxis::X);
			float newelZ = GetCorrectSizeForSegment(Selected_Newel, ESplineMeshAxis::Z);
			float upVectorSize = 0;
			FVector upVector = FVector::ZeroVector;

#pragma region Rail/Mid Section

			//Calculating start position distance for rail/newel
			startPosDistance = (SegmentLength * segmentIndex) +
				SplineBeginOffset +
				SectionLength * sectionIndex;


			startPosDistance = FMath::Clamp(startPosDistance, SplineBeginOffset + (sectionIndex * SectionLength), SplineEndPointDistance - SplineEndOffset);

			//Calculating end position distance for rail/newel
			endPosDistance = SectionLength * sectionIndex +
				UKismetMathLibrary::SelectFloat((SegmentLength * segmentIndex) + correctSegmentLength, SegmentLength * (segmentIndex + 1), canAddMidNewel) +
				SplineBeginOffset;

			endPosDistance = FMath::Clamp(endPosDistance, SplineBeginOffset + (sectionIndex * SectionLength), SplineEndPointDistance - SplineEndOffset);


			//Scale
			scale2D = FVector2D(1.0f, 1.0f);
			if ((endPosDistance - startPosDistance) >= correctSegmentLength * offsetBeforeAttachMesh)
			{
				//Create SplineMeshComponent
				USplineMeshComponent* NewelSplineMesh = UUniversalFunctionLibrary::AddSplineMeshComponent(this, RootComponent, FVector::ZeroVector, FRotator::ZeroRotator, Scaler);
				meshComponents.Add(NewelSplineMesh);

				UStaticMesh* balusterOrNewelSM = NULL;
				if (canAddMidNewel)
				{
					balusterOrNewelSM = Selected_Newel;
				}
				else
				{
					balusterOrNewelSM = Selected_Baluster;
				}

				NewelSplineMesh->SetStaticMesh(balusterOrNewelSM);
				NewelSplineMesh->SetForwardAxis(ESplineMeshAxis::X, false);
				NewelSplineMesh->SetCollisionEnabled(CollisionEnable);
				NewelSplineMesh->SetCollisionObjectType(CollisionType);
				NewelSplineMesh->SetStartScale(scale2D);
				NewelSplineMesh->SetEndScale(scale2D);

				bool canFlip = UUniversalFunctionLibrary::DivisionRemainder((float)middleNewelFlipIndex, 2.0f) == 0;

				float tangentClamp = correctSegmentLength * UKismetMathLibrary::SelectFloat(-1.0f, 1.0f, canFlip);


				float flippedDistance = FMath::FloatSelect(endPosDistance, startPosDistance, canFlip);

				startPos = MainSpline->GetLocationAtDistanceAlongSpline(startPosDistance, localSpace);
				startTangent = MainSpline->GetTangentAtDistanceAlongSpline(startPosDistance, localSpace);
				startTangent = UKismetMathLibrary::ClampVectorSize(startTangent, tangentClamp, tangentClamp);

				flippedDistance = FMath::FloatSelect(startPosDistance, endPosDistance, canFlip);

				endPos = MainSpline->GetLocationAtDistanceAlongSpline(endPosDistance, localSpace);
				endTangent = MainSpline->GetTangentAtDistanceAlongSpline(endPosDistance, localSpace);
				endTangent = UKismetMathLibrary::ClampVectorSize(endTangent, tangentClamp, tangentClamp);

				if (canFlip)
				{
					NewelSplineMesh->SetStartAndEnd(endPos, endTangent, startPos, startTangent, true);
				}
				else
				{
					NewelSplineMesh->SetStartAndEnd(startPos, startTangent, endPos, endTangent, true);
				}
				middleNewelFlipIndex++;
			}
			else
			{
				continue;
			}



			//If can add mid newel then proceed with prop section
			#pragma region PropsSection
			if (canAddMidNewel)
			{
				if (middleNewelPropIndex == MiddleNewel_PropGap && bMiddleNewel_AddProp && IsValid(Selected_Prop_Middle))
				{
					//Needed variables for props
					float propScaledX = GetCorrectSizeForSegment(Selected_Prop_Middle)*GetScaleFromPercentageInt(MiddleNewel_PropScale);
					
					float propScale = GetScaleFromPercentageInt(MiddleNewel_PropScale);
					
					

					//Calculating start position distance for prop
					startPosDistance = ( ( (propScaledX - newelX) / 2.0f ) * -1.0f) +
						(segmentIndex * SegmentLength) +
						(sectionIndex * SectionLength) +
						SplineBeginOffset;
					
					startPosDistance = FMath::Clamp(startPosDistance, SplineBeginOffset+(sectionIndex*SectionLength), SplineEndPointDistance - SplineEndOffset);
						
					//Calculating end position distance for prop
					endPosDistance = UKismetMathLibrary::SelectFloat((segmentIndex * SegmentLength)+ correctSegmentLength 
						, (segmentIndex+1) * SegmentLength, canAddMidNewel) +
						(((propScaledX - newelX) / 2.0f)) + 
						(sectionIndex * SectionLength) +
						SplineBeginOffset;

					endPosDistance = FMath::Clamp(endPosDistance, SplineBeginOffset + (sectionIndex*SectionLength), SplineEndPointDistance - SplineEndOffset);


					//Calculating Upvector
					upVector =MainSpline->GetUpVectorAtDistanceAlongSpline( endPosDistance - ((endPosDistance - startPosDistance) / 2.0f),localSpace);
					upVectorSize = Scaler.Z  * newelZ;
					upVector *= upVectorSize;
					
					//Scale
					scale2D = FVector2D(propScale, propScale);

					
					if ((endPosDistance - startPosDistance) >= correctSegmentLength * offsetBeforeAttachMesh)
					{
						//Create SplineMeshComponent
						USplineMeshComponent* PropSplineMesh = UUniversalFunctionLibrary::AddSplineMeshComponent(this, RootComponent, upVector, FRotator::ZeroRotator, Scaler);
						meshComponents.Add(PropSplineMesh);

						PropSplineMesh->SetStaticMesh(Selected_Prop_Middle);
						PropSplineMesh->SetForwardAxis(ESplineMeshAxis::X, false);
						PropSplineMesh->SetCollisionEnabled(CollisionEnable);
						PropSplineMesh->SetCollisionObjectType(CollisionType);
						PropSplineMesh->SetStartScale(scale2D);
						PropSplineMesh->SetEndScale(scale2D);


						float tangentClamp = UKismetMathLibrary::SelectFloat(propScaledX, correctSegmentLength, canAddMidNewel);

						startPos = MainSpline->GetLocationAtDistanceAlongSpline(startPosDistance, localSpace);
						startTangent = MainSpline->GetTangentAtDistanceAlongSpline(startPosDistance, localSpace);
						startTangent = UKismetMathLibrary::ClampVectorSize(startTangent, tangentClamp, tangentClamp);


						endPos = MainSpline->GetLocationAtDistanceAlongSpline(endPosDistance, localSpace);
						endTangent = MainSpline->GetTangentAtDistanceAlongSpline(endPosDistance, localSpace);
						endTangent = UKismetMathLibrary::ClampVectorSize(endTangent, tangentClamp, tangentClamp);


						PropSplineMesh->SetStartAndEnd(startPos, startTangent, endPos, endTangent, true);

						middleNewelPropIndex = 0;
					}

				}
				else
				{
					middleNewelPropIndex++;
				}
			}
			#pragma endregion
				
			#pragma endregion
		}

	}

	
}









