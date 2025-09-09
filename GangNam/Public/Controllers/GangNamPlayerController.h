// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Widget/WidgetManager.h"
#include "GangNamPlayerController.generated.h"


class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
/**
 *
 */
UCLASS()
class GANGNAM_API AGangNamPlayerController : public APlayerController
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* ControllerMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SkipDialogueAction;

	void OnSkipDialogue(const FInputActionValue& Value);


protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;


public:
	AGangNamPlayerController();

	UPROPERTY()
	UWidgetManager* WidgetManager;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<UBaseTextWidget> BottomDialogueWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<UBaseTextWidget> MiddleDialogueWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<UBaseTextWidget> TopDialogueWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<UFadeWidget> FadeWidgetClass;

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowWidget(TSubclassOf<UUserWidget> WidgetClass, int32 ZOrder);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void RemoveWidget(UUserWidget* WidgetInstance);

};
