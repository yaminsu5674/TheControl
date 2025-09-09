// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameModes/GangNamGameMode.h"
#include "Widget/ColorWidget.h"
#include "Widget/AskWidget.h"
#include "Widget/BaseImageTextWidget.h"
#include "GangNamStationGameMode.generated.h"


class ACrowdCharacter;
/**
 * 
 */
UCLASS()
class GANGNAM_API AGangNamStationGameMode : public AGangNamGameMode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UColorWidget> ScaryWidgetClass;

	UPROPERTY()
	UColorWidget* ScaryWidget;

	UPROPERTY()
	int32 ScaryPoint = 0;

    UPROPERTY()
    bool IsScary = false;

    UPROPERTY()
    int32 Index = 0;

    // Bottom Dialogue Widget
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<UBaseImageTextWidget> BottomDialogueWidgetClass;

    UPROPERTY()
    UBaseImageTextWidget* BottomDialogueWidget;

    // Ask Widget
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<UAskWidget> AskWidgetClass;

    UPROPERTY()
    UAskWidget* AskWidget;

    // Mine Widget
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<UUserWidget> MineWidgetClass;

    UPROPERTY()
    UUserWidget* MineWidget;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<UUserWidget> ChangeWidgetClass;

    UPROPERTY()
    UUserWidget* ChangeWidget;

    UPROPERTY()
    UDataTable* CurrentDialogueTable;

    UPROPERTY()
    TArray<ACrowdCharacter*> CrowdCharacters;

    UPROPERTY()
    ACrowdCharacter* NowCrowd;

    UPROPERTY()
    int32 CrowdCount = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Light", meta = (AllowPrivateAccess = "true"))
    float ScaryIntensity = 0.2f;

    UPROPERTY()
    int32 DialogueCount = 0;

protected:
	virtual void BeginPlay() override;

	virtual void HandleGameplayStep(const FScenarioStep& Step) override;

	virtual void HandleDialogueStep(const FScenarioStep& Step) override;

    void SearchCrowds();

public:


    UPROPERTY(BlueprintReadOnly)
    bool IsMine = false;
    UPROPERTY(BlueprintReadOnly)
    bool IsAsk = false;
    UPROPERTY(BlueprintReadOnly)
    bool IsBottom = false;
	
    void StartMine(UDataTable* DialogueData, ACrowdCharacter* NewCrowd);

    void StartAsk();

    UFUNCTION(BlueprintCallable)
    void StartBottom();

    UFUNCTION(BlueprintCallable)
    void ChangeLogic();

    void StartKill();

    void EndDialogue();

    void PointScary(int8 inValue);

    void CountCrowd();

    bool GetIsBottomTypingFinished();
};
