// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"

#include "GameFramework/Controller.h"

#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

#include "DrawDebugHelpers.h"
#include "Engine.h"



#include "../../Components/InventoryComponent.h"

#include "../../Actors/Interactables/ChestParent.h"
#include "../../Actors/Interactables/DoorParent.h"

// Sets default values
ACharacterBase::ACharacterBase()
{

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 450.0f;
	CameraBoom->SetRelativeRotation(FRotator(-20.0f, 0.0f, 0.0f));


	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComp->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	/*
	USkeletalMeshComponent* SomeMesh = GetMesh();

	SwordMesh->CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwordMesh"));
	SwordMesh->SetupAttachment(SomeMesh);
	SwordMesh->SetRelativeRotation(SwordIdleRotation);

	ShieldMesh->CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShieldMesh"));
	ShieldMesh->SetupAttachment(SomeMesh);
	ShieldMesh->SetRelativeScale3D(ShieldScale);
	ShieldMesh->SetRelativeRotation(ShieldIdleRotation);
	ShieldMesh->SetRelativeLocation(ShieldIdleOffset);
	*/


	TempVisualizer = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Visualizer"));
	TempVisualizer->SetupAttachment(RootComponent);
	TempVisualizer->SetWorldScale3D(FVector(0.75f, 0.75f, 1.25f));

	DebugArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("DebugArrow"));
	DebugArrow->SetupAttachment(TempVisualizer);
	DebugArrow->ArrowSize = 1.5f;
	DebugArrow->ArrowLength = 60.0f;
	DebugArrow->bHiddenInGame = false;

	Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
	Inventory->OnCurrentEquipmentChanged.AddDynamic(this, &ACharacterBase::EquipmentChanged);


	GetCapsuleComponent()->SetCapsuleHalfHeight(60.0f);

	GetCharacterMovement()->GravityScale = 2.0f;
	GetCharacterMovement()->AirControl = 1.0f;


	bUseControllerRotationYaw = false;


	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	NormalScale = GetMesh()->GetRelativeScale3D();

	IdleState = EIdleSubstates::Stand;
	RunState = ERunSubstates::Walk;
	OnIdleStateChanged.Broadcast(IdleState, TEXT("Standing"));
	SetPhysicsStates(EPhysicsStates::Idle);
	OnPhysicsStateChanged.Broadcast(PhysicsState, TEXT("State: Idle"));
	

}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateMovementInput();
	UpdatePhysicsStates(PhysicsState, DeltaTime);
	UpdateScaling(DeltaTime);
	UpdateRotation(DeltaTime);
	UpdateCameraRotation(DeltaTime);
	TripleJumpFlip(DeltaTime);

}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ACharacterBase::InputSprintPressed);
	//PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ACharacterBase::InputSprintReleased);

	// Jump
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacterBase::InputJumpPressed).bExecuteWhenPaused = true;
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacterBase::InputJumpReleased);

	// Rotate Camera
	PlayerInputComponent->BindAction("CameraRotateLeft", IE_Pressed, this, &ACharacterBase::InputCameraRotateLeft);
	PlayerInputComponent->BindAction("CameraRotateRight", IE_Pressed, this, &ACharacterBase::InputCameraRotateRight);
	PlayerInputComponent->BindAction("CameraReset", IE_Pressed, this, &ACharacterBase::InputCameraReset);

	PlayerInputComponent->BindTouch(IE_Pressed, this, &ACharacterBase::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ACharacterBase::TouchStopped);

	// Input
	PlayerInputComponent->BindAxis("MoveForward", this, &ACharacterBase::UpdateForwardInput);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACharacterBase::UpdateRightInput);


}

void ACharacterBase::UpdateForwardInput(float Value)
{
	if (Value > InputThresholdValue
		|| Value < -InputThresholdValue)
	{
		InputForwardValue = Value;
	}
	else
	{
		InputForwardValue = 0.0f;
	}
}

void ACharacterBase::UpdateRightInput(float Value)
{
	if (Value > InputThresholdValue
		|| Value < -InputThresholdValue)
	{
		InputRightValue = Value;
	}
	else
	{
		InputRightValue = 0.0f;
	}
}

void ACharacterBase::InputSprintPressed()
{
	if (!bCanSprint)
	{
		return;
	}
	bIsRunning = true;
	SetRunSubstates(ERunSubstates::Sprint);

	return;
}

void ACharacterBase::InputSprintReleased()
{
	bIsRunning = false;
	SetRunSubstates(ERunSubstates::Walk);

	return;
}

void ACharacterBase::InputJumpPressed()
{
	if (IsValid(ActiveDialogue))
	{
		if (InteractableType == EInteractionTypes::Confirm)
		{
			InputCancelInteraction();
		}
		return;
	}

	if (PhysicsState == EPhysicsStates::Cinematic
		|| PhysicsState == EPhysicsStates::Roll)
	{
		return;
	}

	bJumpStopRequested = false;

	if (PhysicsState == EPhysicsStates::Jump)
	{
		if (JumpState != EJumpSubstates::Rise)
		{
			bJumpRequested = JumpReactionDistance();
			GetWorldTimerManager().SetTimer(JumpRequestHandler, [this]()
			{
				bJumpRequested = false;
			}, JumpRequestTime, false);
		}
		/*
		* Double-Jump logic.. I remembered this being a thing, it's not.
		if (!bJumpRequested)
		{
			if (JumpAirCount <= JumpAirCountMax)
			{
				float JumpOffset = (JumpHeight * JumpAirMultiplier);
				LaunchCharacter(FVector(0.0f, 0.0f, JumpOffset), false, false);
				JumpAirCount += 1;
				JumpComboCount = 0;
				JumpComboMultiplier = 0.0f;
			}
		}
		*/
	}
	if (PhysicsState != EPhysicsStates::Jump)
	{
		bJumpRequested = true;
		CheckJumps();
	}
}

void ACharacterBase::CheckJumps()
{
	bool bDebugBroadcast = false;
	if (bJumpRequested)
	{
		float JumpOffset = JumpHeight;
		if (JumpComboCount <= JumpComboCountMax)
		{
			JumpComboCount += 1;

			if (JumpComboCount == 2)
			{
				JumpComboMultiplier = JumpComboMultiplierIncrement2;
				JumpOffset += (JumpHeight * JumpComboMultiplier);
			}

			if (JumpComboCount == JumpComboCountMax)
			{
				FVector Velocity = GetCharacterMovement()->Velocity;

				bIsTripleJump = true;

				FTimerHandle FlipHandle;
				GetWorldTimerManager().SetTimer(FlipHandle, [this]()
				{
					bIsTripleJump = false;
				}, 5.0f, false);
				
				// A requirement for the Triple-Jump is that there's forward
				// Velocity. So, we zero out down here before the launch
				JumpComboMultiplier = JumpComboMultiplierIncrement3;
				JumpOffset += (JumpHeight * JumpComboMultiplier);
				if (Velocity.X == 0.0f && Velocity.Y == 0.0f)
				{
					JumpOffset = JumpHeight;
				}

				OnLaunchedCharacter.Broadcast(JumpComboCount, JumpComboMultiplier);
				JumpComboCount = 0;
				JumpComboMultiplier = 0.0f;

				bDebugBroadcast = true;
				FTimerHandle ReadOutHandle;
				GetWorldTimerManager().SetTimer(ReadOutHandle, [this]()
				{
					OnLaunchedCharacter.Broadcast(JumpComboCount, JumpComboMultiplier);
				}, 1.0f, false);
			}
		}

		LaunchCharacter(FVector(0.0f, 0.0f, JumpOffset), false, false);
		LaunchVFX();
		bJumpRequested = false;
		if (!bDebugBroadcast)
		{
			OnLaunchedCharacter.Broadcast(JumpComboCount, JumpComboMultiplier);
		}
	}
}

void ACharacterBase::InputJumpReleased()
{
	if (IsValid(ActiveDialogue))
	{
		return;
	}

	if (PhysicsState == EPhysicsStates::Cinematic
		|| PhysicsState == EPhysicsStates::Roll)
	{
		return;
	}

	bJumpStopRequested = true;

	GetWorldTimerManager().SetTimer(JumpStopRequestHandler, [this]()
	{
		if (PhysicsState != EPhysicsStates::Jump)
		{
			bJumpStopRequested = false;
			return;
		}
		FVector CharacterVelocity = GetCharacterMovement()->Velocity;
		if (bJumpStopRequested)
		{
			bJumpStopRequested = false;
			if (CharacterVelocity.Z < 0.0f)
			{
				return;
			}
			CharacterVelocity *= 0.25;
			GetCharacterMovement()->Velocity = CharacterVelocity;
		}

	}, JumpStopRequestTime, false);

}

void ACharacterBase::InputRoll()
{
	if (PhysicsState == EPhysicsStates::Cinematic
		|| PhysicsState == EPhysicsStates::Jump)
	{
		return;
	}

	if (bCanRoll)
	{
		SetPhysicsStates(EPhysicsStates::Roll);
	}
}

void ACharacterBase::InputInteraction()
{
	if (!IsValid(InteractionActor))
	{
		if (IsValid(Inventory))
		{
			if (Inventory->CurrentEquipment == EEquipmentStates::None)
			{
				Inventory->SetCurrentEquipment(EEquipmentStates::SandS);
				return;
			}
			if (Inventory->CurrentEquipment == EEquipmentStates::SandS)
			{
				if (!bIsSwingingSword && !GetWorldTimerManager().IsTimerActive(AttackTimeHandle))
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Attack!"));
					bIsSwingingSword = true;
					GetWorldTimerManager().SetTimer(AttackTimeHandle, [this]()
					{
						bIsSwingingSword = false;
					}, AttackTime, false);
					return;
				}
				else
				{
					bIsSwingingSword = false;
					GetWorldTimerManager().ClearTimer(AttackTimeHandle);
					FTimerHandle AttackResetHandle;
					GetWorldTimerManager().SetTimer(AttackResetHandle, [this]()
					{
						InputInteraction();
					}, 0.05f, false);
					return;
				}
			}
		}
		return;
	}
	else
	{
		if (IsValid(Inventory))
		{
			if (Inventory->CurrentEquipment != EEquipmentStates::None)
			{
				Inventory->SetCurrentEquipment(EEquipmentStates::None);
				return;
			}
		}
	}

	AChestParent* Chest = Cast<AChestParent>(InteractionActor);
	if (IsValid(Chest))
	{
		Chest->OpenChest();
		return;
	}

	ADoorParent* Door = Cast<ADoorParent>(InteractionActor);
	if (IsValid(Door))
	{
		bool bKeyWasUsed = false;
		if (Door->bIsLocked)
		{
			if (!IsValid(ActiveDialogue))
			{
				ActiveDialogue = Door->ActiveDialogue;
				if (IsValid(ActiveDialogue))
				{
					if (Inventory->SmallKeys.Num() > 0)
					{
						bInteractionIsLocked = true;
						FTimerHandle LockedHandle;
						GetWorldTimerManager().SetTimer(LockedHandle, [this]()
						{
							bInteractionIsLocked = false;
						}, 0.1f, false);
					}
					SetInteractionType(EInteractionTypes::Confirm);
					SpawnActiveDialogue();
					return;
				}
			}
			if (!IsValid(Inventory))
			{
				return;
			}
			FString RequiredKey = Door->RequiredKeyName;
			if (Inventory->SmallKeys.Contains(RequiredKey))
			{
				Door->OpenDoor(bKeyWasUsed, RequiredKey);
				if (bKeyWasUsed)
				{
					Inventory->SmallKeys.Remove(RequiredKey);
					UpdateKeyCount();
					Door->OpenDoor(bKeyWasUsed);
				}
			}

			RemoveActiveDialogue();
			ActiveDialogue = nullptr;
			SetInteractionType(EInteractionTypes::None);

		}
		else
		{
			Door->OpenDoor(bKeyWasUsed);
		}
		return;
	}
}

void ACharacterBase::InputCancelInteraction()
{
	if (IsValid(ActiveDialogue))
	{
		RemoveActiveDialogue();
		ActiveDialogue = nullptr;
		if (IsValid(InteractionActor))
		{
			SetInteractionType(PreviousInteractableType);
		}
		else
		{
			SetInteractionType(EInteractionTypes::None);
		}
	}
}

void ACharacterBase::InputCameraRotateLeft()
{
	if (IsValid(ActiveDialogue))
	{
		return;
	}

	if (PhysicsState == EPhysicsStates::Cinematic)
	{
		return;
	}

	float Rotation = CameraBoom->GetRelativeRotation().Yaw;
	CameraSnapTarget = Rotation - CameraSnapAngle;
}

void ACharacterBase::InputCameraRotateRight()
{
	if (IsValid(ActiveDialogue))
	{
		return;
	}

	if (PhysicsState == EPhysicsStates::Cinematic)
	{
		return;
	}

	float Rotation = CameraBoom->GetRelativeRotation().Yaw;
	CameraSnapTarget = Rotation + CameraSnapAngle;
}

void ACharacterBase::InputCameraReset()
{
	if (IsValid(ActiveDialogue))
	{
		return;
	}

	if (PhysicsState == EPhysicsStates::Cinematic)
	{
		return;
	}

	float Rotation = GetMesh()->GetRelativeRotation().Yaw + 90.0f;
	CameraSnapTarget = Rotation;
}

void ACharacterBase::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (IsValid(ActiveDialogue))
	{
		return;
	}

	if (PhysicsState == EPhysicsStates::Cinematic)
	{
		return;
	}

	InputTouchLocation = Location;
	GetWorldTimerManager().SetTimer(SwipeInputHandle, [this]()
	{
		//
	}, 0.5f, false);
}

void ACharacterBase::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (IsValid(ActiveDialogue))
	{
		return;
	}

	if (PhysicsState == EPhysicsStates::Cinematic)
	{
		return;
	}

	bool bSwiped = false;
	if (GetWorldTimerManager().IsTimerActive(SwipeInputHandle))
	{
		bSwiped = true;
	}
	if (bSwiped)
	{
		if (Location.Y > InputTouchLocation.Y + TouchSwipeThresholdValue)
		{
			InputCameraReset();

		}

		if (Location.X > InputTouchLocation.X + TouchSwipeThresholdValue)
		{
			InputCameraRotateRight();

		}

		if (Location.X < InputTouchLocation.X - TouchSwipeThresholdValue)
		{
			InputCameraRotateLeft();

		}
	}

}

void ACharacterBase::UpdateMovementInput()
{
	if (IsValid(ActiveDialogue))
	{
		return;
	}

	if (PhysicsState == EPhysicsStates::Cinematic
		|| PhysicsState == EPhysicsStates::Roll)
	{
		return;
	}

	FVector CharacterVelocity = GetCharacterMovement()->Velocity;
	bool bIsInput = false;

	// Forward
	if (InputForwardValue != 0.0f)
	{
		float Value;
		if (InputForwardValue > 0.0f)
		{
			Value = 1.0f;
		}
		else
		{
			Value = -1.0f;
		}

		FVector Direction = UKismetMathLibrary::GetForwardVector(CameraBoom->GetRelativeRotation());
		AddMovementInput(Direction, Value);
		bIsInput = true;
	}
	else
	{
		if (CharacterVelocity.X != 0.0f)
		{
			AddMovementInput(FVector(1.0f, 0.0f, 0.0f), 0.0f);
		}
	}

	// Right
	if (InputRightValue != 0.0f)
	{
		float Value;
		if (InputRightValue > 0.0f)
		{
			Value = 1.0f;
		}
		else
		{
			Value = -1.0f;
		}
		FVector Direction = UKismetMathLibrary::GetRightVector(CameraBoom->GetRelativeRotation());
		AddMovementInput(Direction, Value);
		bIsInput = true;
	}
	else
	{
		if (CharacterVelocity.Y != 0.0f)
		{
			AddMovementInput(FVector(0.0f, 1.0f, 0.0f), 0.0f);
		}
	}

	bCanRotate = bIsInput;
}

void ACharacterBase::SetPhysicsStates(EPhysicsStates::Type State)
{
	FString StateString = TEXT("State: ");
	ResetSubstates();

	switch (State)
	{
	case EPhysicsStates::Idle:
	{
		JumpAirCount = 0;
		if (bIsRunning)
		{
			bIsRunning = false;
		}

		CheckJumps();

		GetWorldTimerManager().SetTimer(JumpComboHandler, [this]()
		{
			if (PhysicsState == EPhysicsStates::Idle
				|| PhysicsState == EPhysicsStates::Run)
			{
				JumpComboCount = 0;
				JumpComboMultiplier = 0.0f;
				OnLaunchedCharacter.Broadcast(JumpComboCount, JumpComboMultiplier);
			}
		}, JumpComboResetTime, false);

		bIsTripleJump = false;

		StateString += TEXT("Idle");
		break;
	}
	case EPhysicsStates::Run:
	{
		JumpAirCount = 0;

		CheckJumps();

		GetWorldTimerManager().SetTimer(JumpComboHandler, [this]()
		{
			if (PhysicsState == EPhysicsStates::Idle
				|| PhysicsState == EPhysicsStates::Run)
			{
				JumpComboCount = 0;
				JumpComboMultiplier = 0.0f;
				OnLaunchedCharacter.Broadcast(JumpComboCount, JumpComboMultiplier);
			}
		}, JumpComboResetTime, false);

		bIsTripleJump = false;

		StateString += TEXT("Run");
		break;
	}
	case EPhysicsStates::Jump:
	{
		// Do something here
		StateString += TEXT("Jump");
		break;
	}
	case EPhysicsStates::Roll:
	{
		FRotator Rotation = GetMesh()->GetComponentRotation();
		Rotation.Yaw += 90.0f;
		RollDirection = UKismetMathLibrary::GetForwardVector(Rotation);
		GetCharacterMovement()->MaxWalkSpeed = RollSpeed;
		FVector Velocity = RollDirection * RollSpeed;
		Velocity.Z = 0.0f;
		GetCharacterMovement()->Velocity = Velocity;

		bRollStart = true;
		bCanRoll = false;

		FTimerHandle RollHandle;
		GetWorldTimerManager().SetTimer(RollHandle, [this]()
		{
			RollCleanup();
		}, RollTime, false);
		break;
	}
	}

	if (PhysicsState != State)
	{
		OnPhysicsStateChanged.Broadcast(State, StateString);
	}
	PhysicsState = State;
}

void ACharacterBase::SetIdleSubstates(EIdleSubstates::Type State)
{
	FString StateString = TEXT("Substate: ");
	switch (State)
	{
		case EIdleSubstates::Crouch:
		{
			// Do something here
			StateString += TEXT("Crouching");
			break;
		}
		case EIdleSubstates::Stand:
		{
			// Do something here
			StateString += TEXT("Standing");
			break;
		}
	}

	if (SubStateText != StateString)
	{
		SubStateText = StateString;
		OnSubstateChanged.Broadcast(SubStateText);
	}

	if (IdleState != State && PhysicsState == EPhysicsStates::Idle)
	{
		OnIdleStateChanged.Broadcast(State, StateString);
	}
	IdleState = State;
}

void ACharacterBase::SetRunSubstates(ERunSubstates::Type State)
{
	FString StateString = TEXT("Substate: ");
	float Speed = GetCharacterMovement()->MaxWalkSpeed;
	float Accel = GetCharacterMovement()->MaxAcceleration;

	switch (State)
	{
	case ERunSubstates::Crouch:
	{
		// Do something here
		StateString += TEXT("Crouching");
		break;
	}
	case ERunSubstates::Walk:
	{
		if (Speed != WalkSpeed)
		{
			Speed = WalkSpeed;
		}
		if (Accel != WalkAcceleration)
		{
			Accel = WalkAcceleration;
		}
		// Do something here
		StateString += TEXT("Walking");
		break;
	}
	case ERunSubstates::Run:
	{
		if (Speed != WalkSpeed)
		{
			Speed = WalkSpeed;
		}
		if (Accel != WalkAcceleration)
		{
			Accel = WalkAcceleration;
		}
		// Do something here
		StateString += TEXT("Running");
		break;
	}
	case ERunSubstates::Sprint:
	{
		if (Speed != SprintSpeed)
		{
			Speed = SprintSpeed;
		}
		if (Accel != SprintAcceleration)
		{
			Accel = SprintAcceleration;
		}
		// Do something here
		StateString += TEXT("Sprinting");
		break;
	}
	}

	if (SubStateText != StateString)
	{
		SubStateText = StateString;
		OnSubstateChanged.Broadcast(SubStateText);
	}

	if (GetCharacterMovement()->MaxWalkSpeed != Speed)
	{
		GetCharacterMovement()->MaxWalkSpeed = Speed;
	}

	if (GetCharacterMovement()->MaxAcceleration != Accel)
	{
		GetCharacterMovement()->MaxAcceleration = Accel;
	}

	if (RunState != State && PhysicsState == EPhysicsStates::Run)
	{
		OnRunStateChanged.Broadcast(State, StateString);
	}
	RunState = State;
}

void ACharacterBase::SetJumpSubstates(EJumpSubstates::Type State)
{
	FString StateString = TEXT("Substate: ");
	switch (State)
	{
		case EJumpSubstates::Rise:
		{
			// Do something here
			StateString += TEXT("Rising");
			break;
		}
		case EJumpSubstates::Fall:
		{
			// Do something here
			StateString += TEXT("Falling");
			break;
		}
		case EJumpSubstates::Apex:
		{
			// Do something here
			StateString += TEXT("Apex");
			break;
		}
	}

	if (SubStateText != StateString)
	{
		SubStateText = StateString;
		OnSubstateChanged.Broadcast(SubStateText);
	}

	if (JumpState != State)
	{
		OnJumpStateChanged.Broadcast(State, StateString);
	}

	JumpState = State;

}

void ACharacterBase::SetCinematicSubStates(ECinematicSubstates::Type State)
{
	switch (State)
	{
	case ECinematicSubstates::Idle:
	{

		break;
	}
	case ECinematicSubstates::Pickup:
	{

		break;
	}
	case ECinematicSubstates::Grab:
	{

		break;
	}
	}

	CinematicState = State;
}

void ACharacterBase::UpdatePhysicsStates(EPhysicsStates::Type State, float Delta)
{
	FVector CharacterVelocity = GetCharacterMovement()->Velocity;
	float SpeedMax = (WalkSpeed / 3) * 2;

	switch (State)
	{
		case EPhysicsStates::Idle:
		{
			// Do something here
			if (CharacterVelocity != FVector(0.0f, 0.0f, 0.0f))
			{
				if (CharacterVelocity.Z != 0.0f)
				{
					SetPhysicsStates(EPhysicsStates::Jump);
				}
				else
				{
					SetPhysicsStates(EPhysicsStates::Run);
				}
			}

			SetIdleSubstates(EIdleSubstates::Stand);

			break;
		}
		case EPhysicsStates::Run:
		{
			// Do something here
			if (CharacterVelocity != FVector(0.0f, 0.0f, 0.0f))
			{
				if (CharacterVelocity.Z != 0.0f)
				{
					SetPhysicsStates(EPhysicsStates::Jump);
				}
			}
			else
			{
				SetPhysicsStates(EPhysicsStates::Idle);
			}

			if (bIsRunning && bCanSprint)
			{
				SetRunSubstates(ERunSubstates::Sprint);
			}
			else
			{
				if ((CharacterVelocity.X < SpeedMax && CharacterVelocity.X > -SpeedMax)
					&& (CharacterVelocity.Y < SpeedMax && CharacterVelocity.Y > -SpeedMax))
				{
					SetRunSubstates(ERunSubstates::Walk);
				}
				else
				{
					SetRunSubstates(ERunSubstates::Run);
				}
			}

			break;
		}
		case EPhysicsStates::Jump:
		{
			// Do something here
			if (CharacterVelocity != FVector(0.0f, 0.0f, 0.0f))
			{
				if (CharacterVelocity.Z == 0.0f)
				{
					SetPhysicsStates(EPhysicsStates::Run);
					LandingVFX();
				}
			}
			else
			{
				SetPhysicsStates(EPhysicsStates::Idle);
				LandingVFX();
			}

			if ( (CharacterVelocity.Z > -JumpApexRange)
			&& (CharacterVelocity.Z < JumpApexRange) )
			{
				SetJumpSubstates(EJumpSubstates::Apex);
			}
			else
			{
				if (CharacterVelocity.Z > 0.0f)
				{
					SetJumpSubstates(EJumpSubstates::Rise);
				}
				else
				{
					SetJumpSubstates(EJumpSubstates::Fall);
				}
			}

			if (JumpState != EJumpSubstates::Fall && bJumpRequested)
			{
				bJumpRequested = false;
			}
			break;
		}
		case EPhysicsStates::Roll:
		{
			if (CharacterVelocity != FVector(0.0f, 0.0f, 0.0f))
			{
				if (CharacterVelocity.Z != 0.0f)
				{
					SetPhysicsStates(EPhysicsStates::Jump);
					GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
				}
			}
			else
			{
				SetPhysicsStates(EPhysicsStates::Idle);
				GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
			}

			if (!GetCharacterMovement()->IsMovingOnGround())
			{
				SetPhysicsStates(EPhysicsStates::Jump);
				RollCleanup();
			}

			FVector Velocity = RollDirection * RollSpeed;
			Velocity.Z = 0.0f;
			GetCharacterMovement()->Velocity = Velocity;

		}
	}
}

void ACharacterBase::ResetSubstates()
{
	SetIdleSubstates(EIdleSubstates::Stand);
	SetRunSubstates(ERunSubstates::Walk);
	SetJumpSubstates(EJumpSubstates::Rise);
}

void ACharacterBase::RollCleanup()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	if (PhysicsState == EPhysicsStates::Roll
		|| PhysicsState == EPhysicsStates::Jump
		|| PhysicsState == EPhysicsStates::Idle
		|| PhysicsState == EPhysicsStates::Cinematic)
	{
		GetCharacterMovement()->Velocity = FVector::ZeroVector;
	}
	if (PhysicsState == EPhysicsStates::Roll)
	{
		// This is probably fine.
		SetPhysicsStates(EPhysicsStates::Idle);
	}
	if (InputForwardValue != 0.0f || InputRightValue != 0.0f)
	{
		SetPhysicsStates(EPhysicsStates::Run);
		FVector Velocity = GetCharacterMovement()->Velocity;
		Velocity.X = FMath::Clamp(Velocity.X, -1.0f, 1.0f);
		Velocity.Y = FMath::Clamp(Velocity.Y, -1.0f, 1.0f);
		Velocity.Z = FMath::Clamp(Velocity.Z, -1.0f, 1.0f);
		Velocity *= GetCharacterMovement()->MaxWalkSpeed;
		GetCharacterMovement()->Velocity = Velocity;

	}

	bCanRoll = true;
}

bool ACharacterBase::JumpReactionDistance()
{
	FVector Start = GetActorLocation();
	FVector End = Start;
	End.Z -= (GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + JumpRequestDistance);

	FCollisionQueryParams CollisionParams(FName(TEXT("Test!")), true, this);
	CollisionParams.AddIgnoredActor(this);

	FHitResult OutHit(ForceInit);

	bool bIsHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams);

	bool bDraw = (bIsDebug && bDrawJumpRequest);

	if (bIsHit)
	{
		if (OutHit.bBlockingHit)
		{
			if (bDraw)
			{
				DrawDebugLine(GetWorld(), Start, OutHit.Location, FColor::Red,
					false, DebugLineLifeTime, DebugLineDepth, DebugLineThickness);
			}
		}
		return true;
	}

	return false;
}

void ACharacterBase::LandingVFX()
{
	GetMesh()->SetRelativeScale3D(LandSquash);
	ScaleSpeed = SquashSpeed;
}

void ACharacterBase::LaunchVFX()
{
	GetMesh()->SetRelativeScale3D(JumpStretch);
	ScaleSpeed = StretchSpeed;
}

void ACharacterBase::UpdateScaling(float Delta)
{
	FVector Scale = GetMesh()->GetRelativeScale3D();

	if (Scale == NormalScale)
	{
		return;
	}

	Scale.X = UStaticsLibrary::Approach(Scale.X, NormalScale.X, (ScaleSpeed * Delta));
	Scale.Y = UStaticsLibrary::Approach(Scale.Y, NormalScale.Y, (ScaleSpeed * Delta));
	Scale.Z = UStaticsLibrary::Approach(Scale.Z, NormalScale.Z, (ScaleSpeed * Delta));

	GetMesh()->SetRelativeScale3D(Scale);
}

void ACharacterBase::UpdateRotation(float Delta)
{
	if (!bCanRotate || bIsTripleJump)
	{
		return;
	}

	FVector TargetLocation = GetActorLocation() + GetCharacterMovement()->Velocity;
	FVector CurrentLocation = GetActorLocation();

	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(CurrentLocation, TargetLocation);
	FRotator CurrentRotation = GetMesh()->GetComponentRotation();

	TargetRotation.Yaw -= 90.0f;

	TargetRotation.Pitch = 0.0f;
	TargetRotation.Roll = 0.0f;
	CurrentRotation.Pitch = 0.0f;
	CurrentRotation.Roll = 0.0f;


	if (PhysicsState != EPhysicsStates::Idle)
	{
		CurrentRotation = UKismetMathLibrary::RInterpTo(CurrentRotation, TargetRotation, Delta, RotationSpeed);
		GetMesh()->SetWorldRotation(CurrentRotation);
	}
}

void ACharacterBase::TripleJumpFlip(float Delta)
{


	FRotator CurrentRotation = TempVisualizer->GetRelativeRotation();
	FRotator TargetRotation = CurrentRotation;

	if (!bIsTripleJump)
	{
		if (CurrentRotation.Pitch != 0.0f)
		{
			TargetRotation.Pitch = 0.0f;
			CurrentRotation = UKismetMathLibrary::RInterpTo(CurrentRotation, TargetRotation, Delta, 0.0f);
			TempVisualizer->SetRelativeRotation(CurrentRotation);
		}
		return;
	}


	//TargetRotation.Pitch -= (TripleJumpRotationSpeed);
	//CurrentRotation = UKismetMathLibrary::RInterpTo(CurrentRotation, TargetRotation, Delta, 0.0f);
	//TempVisualizer->SetRelativeRotation(CurrentRotation);
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::SanitizeFloat(CurrentRotation.Pitch));

}

void ACharacterBase::UpdateCameraRotation(float Delta)
{
	FRotator CurrentRotation = CameraBoom->GetRelativeRotation();

	if (CurrentRotation.Yaw == CameraSnapTarget)
	{
		return;
	}

	FRotator TargetRotation = CameraBoom->GetRelativeRotation();
	TargetRotation.Yaw = CameraSnapTarget;
	CurrentRotation = UKismetMathLibrary::RInterpTo(CurrentRotation, TargetRotation, Delta, CameraRotationSpeed);

	CameraBoom->SetRelativeRotation(CurrentRotation);
	AddControllerYawInput(CameraSnapTarget);

}

void ACharacterBase::UpdateKeyCount()
{
	int SmallKeyCount = 0;
	if (IsValid(Inventory))
	{
		SmallKeyCount = Inventory->SmallKeys.Num();
		UpdateSmallKeyCount();
	}
}

void ACharacterBase::SetInteractionType(EInteractionTypes::Type Interaction)
{
	PreviousInteractableType = InteractableType;
	InteractableType = Interaction;
	OnInteractionChanged();
}

void ACharacterBase::PickupInteraction(FString NamedItem, int ValueItem, TSubclassOf<AActor> ActorItem)
{
	if (!IsValid(Inventory))
	{
		return;
	}

	FString SmallKey = "Small_Key";
	if (NamedItem == SmallKey)
	{

		Inventory->SmallKeys.Add(NamedItem);
		SetPhysicsStates(EPhysicsStates::Cinematic);
		SetCinematicSubStates(ECinematicSubstates::Collect);
		OnItemCollectStarted();
		CameraPreviousRotation = CameraBoom->GetComponentRotation().Yaw;
		FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CameraComp->GetComponentLocation());

		float Rotation = TargetRotation.Yaw;
		CameraSnapTarget = Rotation;
		FTimerHandle CollectHandle;
		GetWorldTimerManager().SetTimer(CollectHandle, [this]()
		{
			CameraSnapTarget = CameraPreviousRotation;
			CameraPreviousRotation = 0.0f;
			SetPhysicsStates(EPhysicsStates::Idle);
			SetCinematicSubStates(ECinematicSubstates::Idle);
			OnItemCollectFinished();
			UpdateKeyCount();
		}, 5.0f, false);

	}

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, NamedItem);

}

void ACharacterBase::EquipmentChanged()
{
	OnEquipmentChanged();
}

