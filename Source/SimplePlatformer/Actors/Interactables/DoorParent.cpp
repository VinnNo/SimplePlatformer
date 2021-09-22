// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorParent.h"

#include "Components/SphereComponent.h"

#include "Kismet/KismetMathLibrary.h"

#include "../../Pawns/Characters/CharacterBase.h"
#include "../../Data/StaticsLibrary.h"
#include "../../Components/InteractionSphere.h"

// Sets default values
ADoorParent::ADoorParent()
{


	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChestLid"));
	RootComponent = Door;

	if (!bHingeSideLeft)
	{
		FVector Scale = Door->GetComponentScale();
		Scale.X *= -1;
		Door->SetWorldScale3D(Scale);
	}

	InteractionSphere = CreateDefaultSubobject<UInteractionSphere>(TEXT("InteractionSphere"));
	InteractionSphere->SetupAttachment(RootComponent);
	InteractionSphere->OnComponentBeginOverlap.AddDynamic(this, &ADoorParent::OnInteractionRangeEntered);
	InteractionSphere->OnComponentEndOverlap.AddDynamic(this, &ADoorParent::OnInteractionRangeLeft);

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(false);
}

// Called when the game starts or when spawned
void ADoorParent::BeginPlay()
{
	Super::BeginPlay();
	StartingRotation = GetActorRotation();
	TargetRotation = StartingRotation;
	TargetRotation.Yaw = OpenYawMax * InDirection;
}

// Called every frame
void ADoorParent::Tick(float DeltaTime)
{
	if (bIsOpening)
	{
		Super::Tick(DeltaTime);
		FRotator CurrentRotation = GetActorRotation();
		CurrentRotation = UKismetMathLibrary::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 5.0f);
		SetActorRotation(CurrentRotation);
		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::SanitizeFloat(CurrentRotation.Roll));
		if (UKismetMathLibrary::EqualEqual_RotatorRotator(CurrentRotation, TargetRotation, 1.0f))
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Set!"));
			bIsClosed = false;
			bIsOpening = false;
			SetActorTickEnabled(false);
		}
	}

}

void ADoorParent::OnInteractionRangeEntered(UPrimitiveComponent* OverlappedComp,
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

void ADoorParent::OnInteractionRangeLeft(UPrimitiveComponent* OverlappedComp,
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

void ADoorParent::OpenDoor(bool& bKeyUsed, FString KeyName)
{
	if (bIsLocked)
	{
		if (KeyName == RequiredKeyName)
		{
			bIsLocked = false;
			bKeyUsed = true;
			return;
		}
	}
	bKeyUsed = false;
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

