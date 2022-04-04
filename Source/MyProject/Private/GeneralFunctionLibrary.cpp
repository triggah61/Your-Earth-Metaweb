// Fill out your copyright notice in the Description page of Project Settings.


#include "GeneralFunctionLibrary.h"
#include "Containers/UnrealString.h"


FString UGeneralFunctionLibrary::GetRandomIdentifier()
{
	FString result = FString(TEXT(""));

	


	//append numbers to result
	int32 randomInt = FMath::RandRange(42069, 69420);
	result.AppendInt(randomInt);

	//append characters to result
	FString alphabet[] = { "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"};
	TArray<FString> alphabetArray;
	alphabetArray.Append(alphabet, ARRAY_COUNT(alphabet));
	int32 stringLength = 8;

	for (int i = 0; i < stringLength; i++) 
	{

		int32 randomChar = FMath::RandRange(0, alphabetArray.Num() - 1);

		FString gotString = alphabetArray[randomChar];
		const TCHAR* usedChar = *gotString;
		result.AppendChar(TCHAR(*usedChar));
	}



	return result;
}

float UGeneralFunctionLibrary::CountDownTimer(APlayerController* playerController, float currentTime, float time)
{
	float _ceiling = FGenericPlatformMath::CeilToInt(currentTime);
	
	float _newTime = currentTime - time;
	
	if (FGenericPlatformMath::CeilToInt(_newTime) != _ceiling) 
	{
		return FGenericPlatformMath::CeilToInt(_newTime);
	}
	else
	{
		return _ceiling;
	}
}
