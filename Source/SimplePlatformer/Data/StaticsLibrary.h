// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "StaticsLibrary.generated.h"

UENUM(BlueprintType)
namespace EInteractionTypes
{
	enum Type
	{
		None     UMETA(DisplayName = "None"),
		Open     UMETA(DisplayName = "Open"),
		Grab     UMETA(DisplayName = "Grab"),
		Throw    UMETA(DisplayName = "Throw"),
		Drop     UMETA(DisplayName = "Drop"),
		Confirm  UMETA(DisplayName = "Confirm"),
	};
}
/**
 * 
 */
UCLASS(BlueprintType)
class SIMPLEPLATFORMER_API UStaticsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Interpolation")
		static float Approach(float Start, float End, float Shift);

	UFUNCTION(BlueprintCallable, Category ="Equipment")
	static void GetEquipmentByName(FString Name, int Level, UStaticMesh* &Mesh);
};
