// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Data/DialogueLine.h"
#include "Widget/BaseTextWidget.h"
#include "Widget/FadeWidget.h"
#include "WidgetManager.generated.h"

class AGangNamPlayerController;
class UDialogueWidgetBase;
/**
 * 
 */
UCLASS()
class GANGNAM_API UWidgetManager : public UObject
{
	GENERATED_BODY()

	UPROPERTY()
	int32 CurrentLineIndex;

	UPROPERTY()
	UUserWidget* CurrentWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UBaseTextWidget> BottomDialogueWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UBaseTextWidget> MiddleDialogueWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UBaseTextWidget> TopDialogueWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UFadeWidget> FadeWidgetClass;


	UPROPERTY()
	UBaseTextWidget* BottomDialogueWidget;

	UPROPERTY()
	UBaseTextWidget* MiddleDialogueWidget;

	UPROPERTY()
	UBaseTextWidget* TopDialogueWidget;

	UPROPERTY()
	UFadeWidget* FadeWidget;

	UPROPERTY()
	UBaseTextWidget* CurrentDialogueWidget;   

	UPROPERTY()
	UDataTable* CurrentDialogueTable;

	UPROPERTY()
	AGangNamPlayerController* PC;

public:
	UWidgetManager();

	void Init(AGangNamPlayerController* InPC);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowWidget(TSubclassOf<UUserWidget> WidgetClass, int32 ZOrder);

	void RemoveWidget(UUserWidget* WidgetInstance);

	void StartDialogue(UDataTable* DialogueTable);

	void NextDialogue();

	void EndDialogue();

	bool GetBIsTypingFinished();

	UUserWidget* GetCurrentWidget();
	
	UFUNCTION(BlueprintCallable)
	void PlayFadeIn();

	UFUNCTION(BlueprintCallable)
	void PlayFadeOut();


	static constexpr int32 ZORDER_STORY = 0;
	static constexpr int32 ZORDER_HUD = 1;
	static constexpr int32 ZORDER_DIALOGUE = 2;
	static constexpr int32 ZORDER_SETTINGS = 3;
	
};
