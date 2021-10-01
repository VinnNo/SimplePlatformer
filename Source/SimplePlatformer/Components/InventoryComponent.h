// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

UENUM(BlueprintType)
namespace EEquipmentStates
{
	enum Type
	{
		None     UMETA(DisplayName = "None"),
		Sword    UMETA(DisplayName = "Sword"),
		Shield   UMETA(DisplayName = "Shield"),
		SandS    UMETA(DisplayName = "Sword And Shield"),
	};
}

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCurrentEquipmentChanged);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMPLEPLATFORMER_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();


	FCurrentEquipmentChanged OnCurrentEquipmentChanged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
		TEnumAsByte<EEquipmentStates::Type> CurrentEquipment;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Invetory")
		TArray<FString> SmallKeys;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Invetory")
		TArray<FString> LargeKeys;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Invetory")
		int BossKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Invetory")
		int Money = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Invetory")
		int Bombs = 0;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetCurrentEquipment(EEquipmentStates::Type Equipment);
};
