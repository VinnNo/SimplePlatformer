// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupParent.h"
#include "PickupKey.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
namespace EKeyTypes
{
	enum Type
	{
		SmallKey    UMETA(DisplayName = "Small_Key"),
		BigKey      UMETA(DisplayName = "Big_Key"),
		BossKey     UMETA(DisplayName = "Boss_Key"),
		SpecialKey  UMETA(DisplayName = "Special_Key"),
	};
}

UCLASS()
class SIMPLEPLATFORMER_API APickupKey : public APickupParent
{
	GENERATED_BODY()

public:
	APickupKey();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		float RotationSpeed = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		TEnumAsByte<EKeyTypes::Type> KeyType;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//virtual void OnConstruction(const FTransform& Transform) override;
};
