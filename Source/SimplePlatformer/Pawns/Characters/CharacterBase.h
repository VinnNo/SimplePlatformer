// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../../Data/StaticsLibrary.h"
#include "../../UI/Misc/WDialoge.h"
#include "CharacterBase.generated.h"

/* 
* TODO: Rip out all the player specific functions and make a derived player
* class, so that baddies can reuse all this jazz.
*/


UENUM(BlueprintType)
namespace EPhysicsStates
{
	enum Type
	{
		Idle       UMETA(DisplayName = "Idle"),
		Run        UMETA(DisplayName = "Run"),
		Jump       UMETA(DisplayName = "Jump"),
		Cinematic  UMETA(DisplayName = "Cinematic"),
	};
}

UENUM(BlueprintType)
namespace EIdleSubstates
{
	enum Type
	{
		Stand  UMETA(DisplayName = "Stand"),
		Crouch   UMETA(DisplayName = "Crouch"),
	};
}

UENUM(BlueprintType)
namespace ERunSubstates
{
	enum Type
	{
		Walk   UMETA(DisplayName = "Walk"),
		Run    UMETA(DisplayName = "Run"),
		Sprint UMETA(DisplayName = "Sprint"),
		Crouch UMETA(DisplayName = "Crouch"),
	};
}

UENUM(BlueprintType)
namespace EJumpSubstates
{
	enum Type
	{
		Rise  UMETA(DisplayName = "Rise"),
		Fall  UMETA(DisplayName = "Fall"),
		Apex  UMETA(DisplayName = "Apex"),
	};
}

UENUM(BlueprintType)
namespace ECinematicSubstates
{
	enum Type
	{
		Idle    UMETA(DisplayName = "Idle"),
		Pickup  UMETA(DisplayName = "Pickup"),
		Collect UMETA(DisplayName = "Collect"),
		Grab    UMETA(DisplayName = "Grab"),
	};
}

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPhysicsStateChanged,
EPhysicsStates::Type, State,
FString, StateString);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FIdleStateChanged,
EIdleSubstates::Type, State,
FString, StateString);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRunStateChanged,
ERunSubstates::Type, State,
FString, StateString);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FJumpStateChanged,
EJumpSubstates::Type, State,
FString, StateString);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSubstateChanged,
FString, StateString);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FLaunchedCharacter,
int, Combo,
float, Multiplier);

UCLASS()
class SIMPLEPLATFORMER_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* TempVisualizer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Debug, meta = (AllowPrivateAccess = "true"))
		class UArrowComponent* DebugArrow;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
		class UInventoryComponent* Inventory;

	UPROPERTY(BlueprintAssignable)
		FPhysicsStateChanged OnPhysicsStateChanged;

	UPROPERTY(BlueprintAssignable)
		FIdleStateChanged OnIdleStateChanged;

	UPROPERTY(BlueprintAssignable)
		FRunStateChanged OnRunStateChanged;

	UPROPERTY(BlueprintAssignable)
		FJumpStateChanged OnJumpStateChanged;

	UPROPERTY(BlueprintAssignable)
		FSubstateChanged OnSubstateChanged;

	UPROPERTY(BlueprintAssignable)
		FLaunchedCharacter OnLaunchedCharacter;


	// Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
		float InputForwardValue = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
		float InputRightValue = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
		float InputThresholdValue = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
		float TouchSwipeThresholdValue = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
		FVector InputTouchLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
		bool bIsSwipe = false;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enum")
		TEnumAsByte<EPhysicsStates::Type> PhysicsState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enum")
		TEnumAsByte<EIdleSubstates::Type> IdleState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enum")
		TEnumAsByte<ERunSubstates::Type> RunState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enum")
		TEnumAsByte<EJumpSubstates::Type> JumpState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enum")
		TEnumAsByte<ECinematicSubstates::Type> CinematicState;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Walk/Run")
		float WalkSpeed = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Walk/Run")
		float SprintSpeed = 1200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Walk/Run")
		float WalkAcceleration = 512.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Walk/Run")
		float SprintAcceleration = 2048.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Walk/Run")
		bool bIsRunning = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Walk/Run")
		float RotationSpeed = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Walk/Run")
		bool bCanRotate = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Walk/Run")
		bool bCanSprint = true;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "States")
		FString StateText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "States")
		FString SubStateText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jumping")
		float JumpApexRange = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jumping")
		float JumpHeight = 800.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jumping")
		bool bJumpRequested = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jumping")
		bool bJumpStopRequested = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jumping")
		float JumpRequestDistance = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jumping")
		float JumpRequestTime = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jumping")
		float JumpStopRequestTime = 0.05f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jumping")
		int JumpAirCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jumping")
		int JumpAirCountMax = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jumping")
		float JumpAirMultiplier = 0.8f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jumping")
		int JumpComboCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jumping")
		int JumpComboCountMax = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jumping")
		float JumpComboResetTime = 0.2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jumping")
		float JumpComboMultiplierIncrement2 = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jumping")
		float JumpComboMultiplierIncrement3 = 0.65f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jumping")
		float JumpComboMultiplier = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jumping")
		bool bIsTripleJump = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jumping")
		float TripleJumpRotationSpeed = 100.0f;

	FTimerHandle JumpRequestHandler;

	FTimerHandle JumpStopRequestHandler;

	FTimerHandle JumpComboHandler;

	FTimerHandle SwipeInputHandle;


	// Procedural Animations

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Squash/Stretch")
		FVector NormalScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Squash/Stretch")
		FVector JumpStretch = FVector(0.5f, 0.5f, 1.5f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Squash/Stretch")
		FVector LandSquash = FVector(1.0f, 1.0f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Squash/Stretch")
		float StretchSpeed = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Squash/Stretch")
		float SquashSpeed = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Squash/Stretch")
		float ScaleSpeed = 1.0f;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float CameraRotationSpeed = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float CameraSnapAngle = 45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float CameraSnapTarget = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float CameraResetSpeed = 100.0f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
		AActor* InteractionActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
		TEnumAsByte<EInteractionTypes::Type> InteractableType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
		TEnumAsByte<EInteractionTypes::Type> PreviousInteractableType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
		TSubclassOf<class UWDialoge> ActiveDialogue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
		bool bInteractionIsLocked = false;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
		bool bIsDebug = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
		bool bDrawJumpRequest = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
		float DebugLineThickness = 6.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
		float DebugLineLifeTime = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
		uint8 DebugLineDepth = 0;





protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void UpdateForwardInput(float Value);
	void UpdateRightInput(float Value);
	void UpdateMovementInput();
	void InputSprintPressed();
	void InputSprintReleased();
	UFUNCTION(BlueprintCallable)
	void InputJumpPressed();
	UFUNCTION(BlueprintCallable)
	void InputJumpReleased();
	UFUNCTION(BlueprintCallable)
	void InputCameraRotateLeft();
	UFUNCTION(BlueprintCallable)
	void InputCameraRotateRight();
	UFUNCTION(BlueprintCallable)
	void InputCameraReset();

	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	void SetPhysicsStates(EPhysicsStates::Type State);
	void SetIdleSubstates(EIdleSubstates::Type State);
	void SetRunSubstates(ERunSubstates::Type State);
	void SetJumpSubstates(EJumpSubstates::Type State);
	void SetCinematicSubStates(ECinematicSubstates::Type State);
	void UpdatePhysicsStates(EPhysicsStates::Type State, float Delta);
	void ResetSubstates();
	
	void CheckJumps();

	void TripleJumpFlip(float Delta);

	bool JumpReactionDistance();

	void LandingVFX();
	void LaunchVFX();
	void UpdateScaling(float Delta);

	void UpdateRotation(float Delta);

	void UpdateCameraRotation(float Delta);

	UFUNCTION(BlueprintCallable)
	void UpdateKeyCount();

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateSmallKeyCount();

	UFUNCTION(BlueprintImplementableEvent)
	void SpawnActiveDialogue();

	UFUNCTION(BlueprintImplementableEvent)
	void RemoveActiveDialogue();

	UFUNCTION(BlueprintCallable)
	void InputInteraction();

	UFUNCTION(BlueprintCallable)
	void InputCancelInteraction();

	UFUNCTION(BlueprintImplementableEvent)
	void OnInteractionChanged();

	UFUNCTION(BlueprintCallable)
	void SetInteractionType(EInteractionTypes::Type Interaction);

	UFUNCTION(BlueprintImplementableEvent)
	void OnItemCollectStarted();

	UFUNCTION(BlueprintImplementableEvent)
	void OnItemCollectFinished();

	UFUNCTION(BlueprintCallable)
	void PickupInteraction(FString NamedItem, int ValueItem, TSubclassOf<AActor> ActorItem);


};
