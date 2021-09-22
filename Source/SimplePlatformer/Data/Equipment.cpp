// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"


Equipment::Equipment()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		Sword1Object(TEXT("StaticMesh'/Game/Platformer/Equipment/SM_Sword1.SM_Sword1"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		Sword2Object(TEXT("StaticMesh'/Game/Platformer/Equipment/SM_Sword1.SM_Sword2"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		Sword3Object(TEXT("StaticMesh'/Game/Platformer/Equipment/SM_Sword1.SM_Sword2"));

	if (Sword1Object.Succeeded())
		EquipmentSwords.Add(Sword1Object.Object);
	if (Sword2Object.Succeeded())
		EquipmentSwords.Add(Sword2Object.Object);
	if (Sword3Object.Succeeded())
		EquipmentSwords.Add(Sword3Object.Object);

}

Equipment::~Equipment()
{

}
