// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DoorParent.generated.h"

UCLASS()
class SIMPLEPLATFORMER_API ADoorParent : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoorParent();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* Door;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		class UInteractionSphere* InteractionSphere;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		bool bCanInteract = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		bool bIsClosed = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		float OpenYawMax = 91.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		int InDirection = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		bool bIsOpening = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		bool bIsLocked = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		bool bHingeSideLeft = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		FString RequiredKeyName = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		FRotator StartingRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		AActor* PlayerActor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		FRotator TargetRotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		TSubclassOf<class UWDialoge> ActiveDialogue;




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
	void OpenDoor(bool& bKeyUsed, FString KeyName = TEXT("None" ));
};
