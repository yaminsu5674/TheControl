// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Controllers/GangNamPlayerController.h"
#include "Data/ScenarioStep.h"
#include "Blueprint/UserWidget.h"
#include "Characters/SoldierCharacter.h"
#include "GangNamGameMode.generated.h"

UCLASS(minimalapi)
class AGangNamGameMode : public AGameModeBase
{
	GENERATED_BODY()



protected:
    virtual void BeginPlay() override;

    UPROPERTY()
    AGangNamPlayerController* PC;

    UPROPERTY(EditDefaultsOnly, Category = "Scenario")
    UDataTable* ScenarioDataTable;

    void GoToNextStep();

    void ExecuteStep(const FScenarioStep& Step);

    virtual void HandleStoryStep(const FScenarioStep& Step);
    
    virtual void HandleDialogueStep(const FScenarioStep& Step);

    virtual void HandleGameplayStep(const FScenarioStep& Step);

    void HandleCinematicStep(const FScenarioStep& Step);

    void HandleLevelTransitionStep(const FScenarioStep& Step);

    UPROPERTY()
    TArray<ASoldierCharacter*> SoldierCharacters;

    void SearchSoldiers();


public:
	AGangNamGameMode();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    float ModeCameraOrthoWidth = 800.f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
    UAudioComponent* BGMComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
    UAudioComponent* SFXComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    TArray<USoundBase*> SoundList;

    UFUNCTION(BlueprintCallable, Category = "Sound")
    void PlayEffectSound(int32 Index);

    UPROPERTY()
    int32 CurrentStepIndex;

    UFUNCTION(BlueprintCallable)
    void OnStepConditionMet();

    UFUNCTION(BlueprintCallable)
    void OnNextDialogue();

};



