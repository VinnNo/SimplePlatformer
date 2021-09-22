// Fill out your copyright notice in the Description page of Project Settings.


#include "StaticsLibrary.h"
#include "Equipment.h"

float UStaticsLibrary::Approach(float Start, float End, float Shift)
{
	if (Start < End)
	{
		return FMath::Min(Start + Shift, End);
	}
	else
	{
		return FMath::Max(Start - Shift, End);
	}
}

void UStaticsLibrary::GetEquipmentByName(FString Name, int Level, UStaticMesh* &Mesh)
{
	UStaticMesh* OutMesh = nullptr;
	TArray<UStaticMesh*> EquipmentList;
	Equipment* EQ;
	EQ = new Equipment();

	if (Name == TEXT("Swords"))
	{
		switch (Level)
		{
			case 1:
			{
				OutMesh = EQ->EquipmentSwords[0];
				break;
			}
			case 2:
			{
				OutMesh = EQ->EquipmentSwords[1];
				break;
			}
			case 3:
			{
				OutMesh = EQ->EquipmentSwords[2];
				break;
			}
		}

		if (OutMesh != nullptr)
		{
			Mesh = OutMesh;
		}
	}

}