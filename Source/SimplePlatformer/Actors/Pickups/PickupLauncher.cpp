// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupLauncher.h"

#include "Components/SphereComponent.h"

#include "GameFramework/ProjectileMovementComponent.h"

#include "../../Data/StaticsLibrary.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APickupLauncher::APickupLauncher()
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	
	RootComponent = CollisionComp;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionProfileName("NoCollision");
	Mesh->BodyInstance.SetCollisionEnabled(ECollisionEnabled::NoCollision);


	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 400.f;
	ProjectileMovement->MaxSpeed = 400.f;
	ProjectileMovement->bRotationFollowsVelocity = false;
	ProjectileMovement->ProjectileGravityScale = 1.0f;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->Bounciness = 0.8f;


 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APickupLauncher::BeginPlay()
{
	Super::BeginPlay();
	
	if (!IsValid(PickupActorToSpawn))
	{
		return;
	}

	UWorld* const World = GetWorld();
	if (IsValid(World))
	{
		FVector SpawnLocation = GetActorLocation();
		FRotator SpawnRotation;
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		PickupActor = World->SpawnActor<APickupParent>(PickupActorToSpawn,
			SpawnLocation, SpawnRotation, Params);

		if (IsValid(PickupActor))
		{
			PickupActor->AttachToComponent(GetRootComponent(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true));
			PickupActor->LaunchActor = this;
			ScaleMax = PickupActor->SpawnScale;
			PickupActor->SetActorScale3D(ScaleStart);
		}
	}
}

// Called every frame
void APickupLauncher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	bool bShouldDestroy = false;

	if (UKismetMathLibrary::Vector_IsNearlyZero(ProjectileMovement->Velocity))
	{
		Destroy();

		/*
		if (IsValid(PickupActor))
		{
			PickupActor->Destroy();
		}
		UWorld* const World = GetWorld();
		if (IsValid(World))
		{
			FVector SpawnLocation = GetActorLocation();
			FRotator SpawnRotation;
			FActorSpawnParameters Params;
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			PickupActor = World->SpawnActor<APickupParent>(PickupActorToSpawn,
				SpawnLocation, SpawnRotation, Params);

			Destroy();
		}
		*/
	}

	SetActorRotation(FRotator::ZeroRotator);

	if (!IsValid(PickupActor))
	{
		return;
	}

	float CurrentValue = PickupActor->GetActorScale3D().X;
	float TargetValue = ScaleMax.X;
	FVector Scale;
	CurrentValue = UStaticsLibrary::Approach(CurrentValue, TargetValue, (ScalingSpeed * DeltaTime));
	Scale = FVector(CurrentValue, CurrentValue, CurrentValue);
	PickupActor->SetActorScale3D(Scale);

}

