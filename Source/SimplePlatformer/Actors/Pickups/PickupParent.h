// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupParent.generated.h"

UENUM(BlueprintType)
namespace EPickupType
{
	enum Type
	{
		Key     UMETA(DisplayName = "Key"),
		Health  UMETA(DisplayName = "Health"),
		Item    UMETA(DisplayName = "Item"),
		Money   UMETA(DisplayName = "Money"),
	};
}


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPickupRangeEntered,
ACharacterBase*, Character,
EPickupType::Type, PickupType);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPickupRangeLeft,
ACharacterBase*, Character,
EPickupType::Type, PickupType);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPickupCollision,
ACharacterBase*, Character,
EPickupType::Type, PickupType);


UCLASS()
class SIMPLEPLATFORMER_API APickupParent : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupParent();


	UPROPERTY(BlueprintAssignable)
	FPickupRangeEntered OnPickupRangeEntered;

	UPROPERTY(BlueprintAssignable)
	FPickupRangeLeft OnPickupRangeLeft;

	UPROPERTY(BlueprintAssignable)
	FPickupCollision OnPickupCollided;


	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* InteractionShphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Rotation, meta = (AllowPrivateAccess = "true"))
		class URotatingMovementComponent* RotatingMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		AActor* LaunchActor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		ACharacterBase* PlayerCharacter = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		TEnumAsByte<EPickupType::Type> PickupType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		FString ItemName = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		int ItemValue = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		TSubclassOf<AActor> ItemActor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		FVector SpawnScale = FVector(1.0f, 1.0f, 1.0f);




protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnInteractionRangeEntered(UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& SweepResult);

	UFUNCTION()
		void OnInteractionRangeLeft(class UPrimitiveComponent* OverlappedComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);

	UFUNCTION()
		void OnPickupOverlapped(UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
		void OnPickedUp();


	FORCEINLINE class UStaticMeshComponent* GetMesh() const { return Mesh;  }
	FORCEINLINE class USphereComponent* GetSphere() const { return InteractionShphere;  }
	FORCEINLINE class URotatingMovementComponent* GetRotatingMovement() const { return RotatingMovement; }
};
