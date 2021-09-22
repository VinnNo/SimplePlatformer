// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupKey.h"

#include "GameFramework/RotatingMovementComponent.h"


APickupKey::APickupKey()
{
	FRotator MeshRotation = FRotator::ZeroRotator;
	MeshRotation.Roll = 90.0f;
	GetMesh()->SetRelativeRotation(MeshRotation);

	FRotator Movement;
	Movement.Yaw = RotationSpeed;
	GetRotatingMovement()->RotationRate = Movement;

	//OnPickupCollided.AddDynamic(this, &APickupKey::PickupCollision);

}

void APickupKey::BeginPlay()
{
	Super::BeginPlay();
	PickupType = EPickupType::Key;
}

/*
void APickupKey::OnConstruction(const FTransform& Transform)
{
	//ItemName = UEnum::GetValueAsString(KeyType);
}
*/
