// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChestParent.generated.h"

UCLASS()
class SIMPLEPLATFORMER_API AChestParent : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChestParent();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* ChestBase;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* ChestLid;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		class UInteractionSphere* InteractionSphere;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		bool bCanInteract = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		bool bIsClosed = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		float OpenRollMax = -90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		bool bIsOpening  = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		TSubclassOf<AActor> SpawnedLoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		AActor* PlayerActor = nullptr;

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

	UFUNCTION(BlueprintCallable)
	void OpenChest();

	UFUNCTION(BlueprintImplementableEvent)
	void SpawnLoot();

};
