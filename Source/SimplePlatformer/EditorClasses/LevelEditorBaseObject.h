// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelEditorBaseObject.generated.h"

UCLASS()
class SIMPLEPLATFORMER_API ALevelEditorBaseObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelEditorBaseObject();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* Mesh;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMesh* BaseMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		TArray<AActor*> PaintedMeshes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Snapping")
		bool bUseSnap = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Snapping")
		int SnapFloorLevel = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Snapping")
		bool bSnapLevelWithFloorSpacing = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Snapping")
		float FloorSpaceing = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Snapping")
		bool bSnapToFloorAtLevel = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Snapping")
		FVector PreviousLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Snapping")
		bool bUseActorLocationInsteadOfCenter = false;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Painting")
		bool bSetToLastPaint = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Painting")
		FVector TargetLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Painting")
		AActor* LastSpawnedActor = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Painting")
		FVector LastSpawnedLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Painting")
		FRotator LastSpawnedRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Painting")
		TArray<FVector> PaintedLocations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Snapping", Meta = (MakeEditWidget = true))
		FVector PainterWidget;

	bool bInit = true;
	bool bCanSpawn = true;
	float SpawnTime = 0.1f;
	FTimerHandle SpawnActorTimerHandle;









protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	void ResetPainterWidget(bool bAtPaintLocation = false);

	void ResetLastSpawnLocation();

	bool CheckPaintLocations(FVector InLocation);

};
