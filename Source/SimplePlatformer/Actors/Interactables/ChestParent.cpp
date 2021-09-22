// Fill out your copyright notice in the Description page of Project Settings.


#include "ChestParent.h"

//#include "Components/SphereComponent.h"

#include "Kismet/KismetMathLibrary.h"

#include "../../Pawns/Characters/CharacterBase.h"

#include "../../Data/StaticsLibrary.h"
#include "../../Components/InteractionSphere.h"

// Sets default values
AChestParent::AChestParent()
{

	ChestBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChestBase"));
	RootComponent = ChestBase;

	ChestLid = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChestLid"));
	ChestLid->SetupAttachment(RootComponent);

	InteractionSphere = CreateDefaultSubobject<UInteractionSphere>(TEXT("InteractionSphere"));
	InteractionSphere->SetupAttachment(RootComponent);
	InteractionSphere->OnComponentBeginOverlap.AddDynamic(this, &AChestParent::OnInteractionRangeEntered);
	InteractionSphere->OnComponentEndOverlap.AddDynamic(this, &AChestParent::OnInteractionRangeLeft);



 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(false);

}

// Called when the game starts or when spawned
void AChestParent::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChestParent::Tick(float DeltaTime)
{
	if (bIsOpening)
	{
		Super::Tick(DeltaTime);
		FRotator CurrentRotation = ChestLid->GetComponentRotation();
		FRotator TargetRotation;
		TargetRotation.Roll = OpenRollMax - 1.0f;
		CurrentRotation = UKismetMathLibrary::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 5.0f);
		ChestLid->SetRelativeRotation(CurrentRotation);
		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::SanitizeFloat(CurrentRotation.Roll));
		if (CurrentRotation.Roll <= OpenRollMax)
		{
			bIsClosed = false;
			bIsOpening = false;
			SpawnLoot();
			SetActorTickEnabled(false);
		}
	}
}

void AChestParent::OnInteractionRangeEntered(UPrimitiveComponent* OverlappedComp, 
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsClosed)
	{
		return;
	}
	ACharacterBase* Character = Cast<ACharacterBase>(OtherActor);
	if (IsValid(Character))
	{
		PlayerActor = OtherActor;
		bCanInteract = true;
		Character->InteractionActor = this;
		Character->SetInteractionType(InteractionSphere->InteractionType);
	}
}

void AChestParent::OnInteractionRangeLeft(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACharacterBase* Character = Cast<ACharacterBase>(OtherActor);
	if (IsValid(Character))
	{
		bCanInteract = false;
		Character->InteractionActor = nullptr;
		Character->SetInteractionType(EInteractionTypes::None);
	}
}

void AChestParent::OpenChest()
{
	if (bIsClosed && !bIsOpening)
	{
		bIsOpening = true;
		SetActorTickEnabled(true);
		ACharacterBase* Character = Cast<ACharacterBase>(PlayerActor);
		if (IsValid(Character))
		{
			Character->SetInteractionType(EInteractionTypes::None);
		}
	}
}


