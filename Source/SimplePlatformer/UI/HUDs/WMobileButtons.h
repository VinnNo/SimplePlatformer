// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/UMG.h"
#include "../../Pawns/Characters/CharacterBase.h"
#include "../../Data/StaticsLibrary.h"
#include "WMobileButtons.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLEPLATFORMER_API UWMobileButtons : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UWMobileButtons(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetItems", meta = (BindWidget))
		class UCanvasPanel* Canvas = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buttons", meta = (BindWidget))
		class UButton* ActionAButton = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buttons", meta = (BindWidget))
		class UButton* ActionBButton = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buttons", meta = (BindWidget))
		class UImage* ActionAImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buttons", meta = (BindWidget))
		class UImage* ActionABGImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buttons", meta = (BindWidget))
		class UImage* ActionBImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buttons", meta = (BindWidget))
		class UTextBlock* ActionASupportText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buttons", meta = (BindWidget))
		class UTextBlock* ActionBSupportText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		class ACharacterBase* PlayerCharacter = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Indicators")
		TEnumAsByte<EInteractionTypes::Type> InteractionType;

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void InputButtonAPressed();
	UFUNCTION(BlueprintCallable)
	void InputButtonAReleased();

	UFUNCTION(BlueprintCallable)
	void InputButtonBPressed();
	UFUNCTION(BlueprintCallable)
	void InputButtonBReleased();
	UFUNCTION(BlueprintCallable)
	void UpdateBSupportImage();

	UFUNCTION(BlueprintCallable)
	void UpdateInteractionType(EInteractionTypes::Type Interaction, bool bSetAToCancel = false);

	void GetPlayerCharacter();
};
