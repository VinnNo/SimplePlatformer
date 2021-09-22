// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/UMG.h"
#include "WInGameHud.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLEPLATFORMER_API UWInGameHud : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetItems", meta =(BindWidget))
		class UCanvasPanel* Canvas = nullptr; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetItems", meta = (BindWidget))
		class UImage* SmallKeyImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetItems", meta = (BindWidget))
		class UImage* SmallKeyBGImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetItems", meta = (BindWidget))
		class UTextBlock* SmallKeyCountLabel = nullptr;

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
		void UpdateSmallKeyCount(int Value);
};
