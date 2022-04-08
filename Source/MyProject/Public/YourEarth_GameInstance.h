// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "YourEarth_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UYourEarth_GameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	virtual void Init() override;

	void BeginLoadingScreen(const FString& MapName);

	void EndLoadingScreen(UWorld* InLoadedWorld);
};
