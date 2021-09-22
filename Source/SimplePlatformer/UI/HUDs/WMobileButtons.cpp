// Fill out your copyright notice in the Description page of Project Settings.


#include "WMobileButtons.h"

#include "Kismet/GameplayStatics.h"

#include "Engine.h"

UWMobileButtons::UWMobileButtons(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UWMobileButtons::NativeConstruct()
{
	if (ActionAButton)
	{
		ActionAButton->OnPressed.AddDynamic(this, &UWMobileButtons::InputButtonAPressed);
	}

	if (ActionAButton)
	{
		ActionAButton->OnReleased.AddDynamic(this, &UWMobileButtons::InputButtonAReleased);
	}

	if (ActionBButton)
	{
		ActionBButton->OnPressed.AddDynamic(this, &UWMobileButtons::InputButtonBPressed);
	}

	InteractionType = EInteractionTypes::Drop;
	UpdateInteractionType(EInteractionTypes::None);
}

void UWMobileButtons::InputButtonAPressed()
{
	if (!IsValid(PlayerCharacter))
	{
		GetPlayerCharacter();
	}
	PlayerCharacter->InputJumpPressed();
}

void UWMobileButtons::InputButtonAReleased()
{
	if (!IsValid(PlayerCharacter))
	{
		GetPlayerCharacter();
	}
	PlayerCharacter->InputJumpReleased();
}

void UWMobileButtons::InputButtonBPressed()
{
	if (!IsValid(PlayerCharacter))
	{
		GetPlayerCharacter();
	}
	PlayerCharacter->InputInteraction();
}

void UWMobileButtons::InputButtonBReleased()
{

}

void UWMobileButtons::UpdateBSupportImage()
{

}

void UWMobileButtons::UpdateInteractionType(EInteractionTypes::Type Interaction, bool bSetAToCancel)
{
	if (InteractionType == Interaction
	|| ActionBSupportImage == nullptr)
	{
		return;
	}

	FLinearColor ColorVisible = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
	FLinearColor ColorHidden = FLinearColor(1.0f, 1.0f, 1.0f, 0.0f);
	//FSlateColor ColorVisible(1.0f, 1.0f, 1.0f, 1.0f);// = FSlateColor(1.0f, 1.0f, 1.0f, 1.0f);
	//FSlateColor ColorHidden = FSlateColor(1.0f, 1.0f, 1.0f, 0.0f);

	ActionASupportImage->SetColorAndOpacity(ColorHidden);

	switch (Interaction)
	{
		case EInteractionTypes::None:
		{
			ActionBSupportImage->SetColorAndOpacity(ColorHidden);
			break;
		}
		case EInteractionTypes::Open:
		{
			ActionBSupportImage->SetBrushFromTexture(OpenTexture);
			break;
		}
		case EInteractionTypes::Grab:
		{
			ActionBSupportImage->SetBrushFromTexture(GrabTexture);
			break;
		}
		case EInteractionTypes::Drop:
		{
			ActionBSupportImage->SetBrushFromTexture(DropTexture);
			break;
		}
		case EInteractionTypes::Throw:
		{
			ActionBSupportImage->SetBrushFromTexture(ThrowTexture);
			break;
		}
		case EInteractionTypes::Confirm:
		{
			ActionBSupportImage->SetBrushFromTexture(OpenTexture);
			ActionASupportImage->SetBrushFromTexture(CancelTexture);
			ActionASupportImage->SetColorAndOpacity(ColorVisible);
		}
	}

	if (Interaction != EInteractionTypes::None)
	{
		ActionBSupportImage->SetColorAndOpacity(ColorVisible);
	}
}

void UWMobileButtons::GetPlayerCharacter()
{

	UWorld* const World = GetWorld();
	if (World)
	{
		APawn* PlayerPawn = World->GetFirstPlayerController()->GetPawn();
		ACharacterBase* Character = Cast<ACharacterBase>(PlayerPawn);
		if (Character)
		{
			PlayerCharacter = Character;
		}
	}

}
