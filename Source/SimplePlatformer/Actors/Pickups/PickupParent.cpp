// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupParent.h"

#include "Components/SphereComponent.h"

#include "../../Pawns/Characters/CharacterBase.h"

#include "GameFramework/RotatingMovementComponent.h"

// Sets default values
APickupParent::APickupParent()
{
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &APickupParent::OnPickupOverlapped);

	InteractionShphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
	InteractionShphere->SetupAttachment(RootComponent);

	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovement"));
	RotatingMovement->RotationRate = FRotator(0.0f, 0.0f, 0.0f);


 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APickupParent::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickupParent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickupParent::OnInteractionRangeEntered(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacterBase* Character = Cast<ACharacterBase>(OtherActor);
	if (IsValid(Character))
	{
		OnPickupRangeEntered.Broadcast(Character, PickupType);
	}
}

void APickupParent::OnInteractionRangeLeft(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACharacterBase* Character = Cast<ACharacterBase>(OtherActor);
	if (IsValid(Character))
	{
		//OnPickupRangeLeft.Broadcast(Character, PickupType);
	}
}

void APickupParent::OnPickupOverlapped(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacterBase* Character = Cast<ACharacterBase>(OtherActor);
	if (IsValid(Character))
	{
		PlayerCharacter = Character;
		OnPickupCollided.Broadcast(Character, PickupType);
		OnPickedUp();
	}
}

void APickupParent::OnPickedUp()
{

	if (!IsValid(PlayerCharacter))
	{
		return;
	}


	switch (PickupType)
	{
	// Health or money
		case EPickupType::Key:
		{
			break;
		}
		
		case EPickupType::Item:
		{
			break;
		}

		case EPickupType::Health:
		{

			break;
		}

		case EPickupType::Money:
		{

			break;
		}
	}

	if (IsValid(LaunchActor))
	{
		LaunchActor->Destroy();
	}

	PlayerCharacter->PickupInteraction(ItemName, ItemValue, ItemActor);
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, ItemName);

	Destroy();
}

