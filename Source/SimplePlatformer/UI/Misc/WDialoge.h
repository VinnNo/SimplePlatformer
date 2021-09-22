// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/UMG.h"
#include "WDialoge.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLEPLATFORMER_API UWDialoge : public UUserWidget
{
	GENERATED_BODY()

public:
	UWDialoge(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetItems", meta = (BindWidget))
		class UCanvasPanel* Canvas = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetItems", meta = (BindWidget))
		class UImage* BG1Image = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetItems", meta = (BindWidget))
		class UImage* BG2Image = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetItems", meta = (BindWidget))
		class UTextBlock* TextHeaderLabel = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetItems", meta = (BindWidget))
		class UTextBlock* TextBodyLabel = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetItems", meta = (BindWidget))
		class UImage* SupportingImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetItems", meta = (BindWidget))
		class UBackgroundBlur* BGBlur = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior")
		bool bBuildBodyTextWithDelay = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior")
		float BuildBodySpeed = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior")
		FString BuildBodyFromString;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior")
		FString BuildBodyToString;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior")
		TArray<FString> BodyDialogueOptions;

	virtual void NativeConstruct() override;
	
};
