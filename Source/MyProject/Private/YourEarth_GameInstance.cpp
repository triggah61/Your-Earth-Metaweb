// Fill out your copyright notice in the Description page of Project Settings.


#include "YourEarth_GameInstance.h"
#include "UnrealClient.h"

void UYourEarth_GameInstance::Init()
{
	Super::Init();

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UYourEarth_GameInstance::BeginLoadingScreen);
	//FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UYourEarth_GameInstance::EndLoadingScreen);
	//Later, use this for loading screen 
	//https://nerivec.github.io/old-ue4-wiki/pages/loading-screen.html
}



void UYourEarth_GameInstance::BeginLoadingScreen(const FString& MapName)
{
	UE_LOG(LogTemp, Warning, TEXT("This is a test. Loading screen to client viewport."));

	
}

void UYourEarth_GameInstance::EndLoadingScreen(UWorld* InLoadedWorld)
{
	UE_LOG(LogTemp, Warning, TEXT("This is a test. Remove loading screen."));
}