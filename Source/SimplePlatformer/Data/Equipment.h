// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class SIMPLEPLATFORMER_API Equipment
{
public:
	Equipment();
	~Equipment();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swords")
		TArray<UStaticMesh*> EquipmentSwords;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SShields")
		TArray<UStaticMesh*> EquipmentShields;
};
