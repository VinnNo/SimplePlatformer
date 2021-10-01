// Fill out your copyright notice in the Description page of Project Settings.


#include "WMobileButtons.h"

#include "Kismet/GameplayStatics.h"

#include "Styling/SlateColor.h"

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
	|| ActionASupportText == nullptr
	|| ActionBSupportText == nullptr)
	{
		return;
	}
	ActionASupportText->SetRenderOpacity(0.0f);

	switch (Interaction)
	{
		case EInteractionTypes::None:
		{
			ActionBSupportText->SetRenderOpacity(0.0f);
			break;
		}
		case EInteractionTypes::Open:
		{
			ActionBSupportText->SetText(FText::FromString(TEXT("Open")));
			break;
		}
		case EInteractionTypes::Grab:
		{
			ActionBSupportText->SetText(FText::FromString(TEXT("Grab")));
			break;
		}
		case EInteractionTypes::Drop:
		{
			ActionBSupportText->SetText(FText::FromString(TEXT("Drop")));
			break;
		}
		case EInteractionTypes::Throw:
		{
			ActionBSupportText->SetText(FText::FromString(TEXT("Throw")));
			break;
		}
		case EInteractionTypes::Confirm:
		{
			ActionBSupportText->SetText(FText::FromString(TEXT("Open")));
			ActionASupportText->SetText(FText::FromString(TEXT("Cancel")));
			ActionASupportText->SetRenderOpacity(1.0f);

		}
	}

	if (Interaction != EInteractionTypes::None)
	{
		ActionBSupportText->SetRenderOpacity(1.0f);
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
