// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelEditorBaseObject.h"
#include "Components/StaticMeshComponent.h"

#include "Engine.h"
#include "Engine/StaticMeshActor.h"
//#include "LevelEditor.h"
//#include "Editor.h"

// Sets default values
ALevelEditorBaseObject::ALevelEditorBaseObject()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALevelEditorBaseObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALevelEditorBaseObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelEditorBaseObject::OnConstruction(const FTransform& Transform)
{
	UStaticMesh* CurrentMesh = Mesh->GetStaticMesh();
	FBox PaintBounds;
	FVector PaintSize;

	if (bInit)
	{
		ResetPainterWidget();
		bInit = false;
	}

	if (PaintedLocations.Num() == 0)
	{
		LastSpawnedLocation = FVector(0.0f, 0.0f, 0.0f);
	}

	if (bSetToLastPaint)
	{
		if (LastSpawnedLocation == FVector(0.0f, 0.0f, 0.0f)
			|| PaintedLocations.Num() == 0)
		{
			ResetPainterWidget();
		}
		else
		{
			ResetPainterWidget(true);
		}
		bSetToLastPaint = false;
	}

	if (BaseMesh != nullptr)
	{
		Mesh->SetStaticMesh(BaseMesh);
	}

	if (bUseSnap)
	{
		float SnapMultiplier = 200.0f;
		if (bSnapLevelWithFloorSpacing)
		{
			SnapMultiplier += FloorSpaceing;
		}
		float SnapLevel = SnapFloorLevel;

		SnapLevel *= SnapMultiplier;

		if (bSnapToFloorAtLevel)
		{
			if (SnapFloorLevel == 0)
			{
				SnapLevel = SnapFloorLevel + FloorSpaceing;
			}
			else
			{
				SnapLevel = (SnapFloorLevel * SnapMultiplier) + FloorSpaceing;
			}
		}

		FVector Location = GetActorLocation();
		Location.Z = SnapLevel;
		SetActorLocation(Location);



		if (PainterWidget.Z != 0.0f)
		{
			PainterWidget.Z = 0.0f;
		}

		// Paint Meshes
		if (CurrentMesh != nullptr)
		{
			PaintBounds = CurrentMesh->GetBoundingBox();
			FVector PaintBoundsOffSet(-2.5f, -2.5f, -2.5f);
			PaintSize = PaintBounds.GetSize() + PaintBoundsOffSet;

			FVector PainterLocation = GetActorLocation() + PainterWidget;
			FVector RefLoc;

			if (LastSpawnedLocation == FVector(0.0f, 0.0f, 0.0f))
			{
				RefLoc = LastSpawnedLocation;
			}
			else
			{
				RefLoc = GetActorLocation();
			}

			bool bSpawn = false;
			bool bSpawnX = false;
			bool bSpawnY = false;

			bool bLastSpawnEmpty = (LastSpawnedLocation == FVector(0.0f, 0.0f, 0.0f));
			
			float PaintX = PainterWidget.X;
			float PaintY = PainterWidget.Y;

			if (PaintX < 0.0f)
			{
				PaintX *= -1;
			}
			if (PaintY < 0.0f)
			{
				PaintY *= -1;
			}
			float ComparisonX = PaintX / PaintSize.X;
			float ComparisonY = PaintY / PaintSize.Y;
			if (round(ComparisonX) != ComparisonX)
			{
				return;
			}
			if (round(ComparisonY) != ComparisonY)
			{
				return;
			}

			// Along X
			if ((PainterLocation.X >= RefLoc.X + PaintSize.X)
			|| (PainterLocation.X <= RefLoc.X - PaintSize.X))
			{

				bSpawnX = true;
				if (PainterWidget.Y == LastSpawnedLocation.Y)
				{
					bSpawnY = true;
				}

					//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::SanitizeFloat(Comparison));

			}
			// Along Y
			if ((PainterLocation.Y >= RefLoc.Y + PaintSize.Y)
			|| (PainterLocation.Y <= RefLoc.Y - PaintSize.Y))
			{

				

				bSpawnY = true;
				if (PainterWidget.X == LastSpawnedLocation.X)
				{
					bSpawnX = true;
				}
				//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::SanitizeFloat(Comparison));

			}

			bSpawn = (bSpawnX && bSpawnY);

			bool bStartingLocation = (PainterWidget == FVector(0.0f, 0.0f, 0.0f));

			if (GetWorldTimerManager().IsTimerActive(SpawnActorTimerHandle)
				|| IsValid(LastSpawnedActor) || !bSpawn || bStartingLocation)
			{
				return;
			}

			// Well, We've made it this far

			UWorld* const World = GetWorld();
			if (World != nullptr)
			{
				FVector SpawnLocation = GetActorLocation() + PainterWidget;
				FRotator SpawnRotation;
				FActorSpawnParameters Params;
				Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;

				if (CheckPaintLocations(SpawnLocation))
				{
					return;
				}

				AStaticMeshActor* SpawnedActor;
				SpawnedActor = World->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), SpawnLocation, SpawnRotation, Params);
				if (IsValid(SpawnedActor))
				{
					SpawnedActor->GetStaticMeshComponent()->SetStaticMesh(CurrentMesh);
					PaintedLocations.Add(SpawnLocation);
					LastSpawnedActor = SpawnedActor;
					LastSpawnedLocation = PainterWidget;
					LastSpawnedRotation = FRotator(0.0f, 0.0f, 0.0f);
						
					GetWorldTimerManager().SetTimer(SpawnActorTimerHandle, [this]()
					{
						//
						if (IsValid(LastSpawnedActor))
						{
							LastSpawnedActor->SetActorRotation(LastSpawnedRotation);
							LastSpawnedActor = nullptr;
						}
					}, SpawnTime, false);
						
				}
			}

		}
	}
}

void ALevelEditorBaseObject::ResetPainterWidget(bool bAtPaintLocation)
{
	UStaticMesh* CurrentMesh = Mesh->GetStaticMesh();
	FBox PaintBounds;
	if (bAtPaintLocation)
	{
		PainterWidget = LastSpawnedLocation;
		return;
	}
	if (CurrentMesh != nullptr)
	{
		PainterWidget = PaintBounds.GetCenter();
		PainterWidget.Y = 0.0f;
		LastSpawnedLocation = PainterWidget;
	}
}

void ALevelEditorBaseObject::ResetLastSpawnLocation()
{
	LastSpawnedLocation = FVector(0.0f, 0.0f, 0.0f);
}

bool ALevelEditorBaseObject::CheckPaintLocations(FVector InLocation)
{
	bool bActorAtLocation = false;
	if (PaintedLocations.Num() > 0)
	{
		for (int Itr = 0; Itr < PaintedLocations.Num(); ++Itr)
		{
			if (InLocation == PaintedLocations[Itr])
			{
				return true;
			}
		}
	}
	return bActorAtLocation;
}

