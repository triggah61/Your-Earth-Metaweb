// Fill out your copyright notice in the Description page of Project Settings.


#include "COBA_MinionCharacter.h"

// Sets default values
ACOBA_MinionCharacter::ACOBA_MinionCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACOBA_MinionCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACOBA_MinionCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	UE_LOG(LogTemp, Warning, TEXT("I'm a minion. Have fun with C++. Testing compilation.1234567"));
}

// Called to bind functionality to input
void ACOBA_MinionCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

