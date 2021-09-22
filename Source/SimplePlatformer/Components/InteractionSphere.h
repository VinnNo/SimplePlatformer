// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "../Data/StaticsLibrary.h"
#include "InteractionSphere.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLEPLATFORMER_API UInteractionSphere : public USphereComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
		TEnumAsByte<EInteractionTypes::Type> InteractionType;


};
