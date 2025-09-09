// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameModes/GangNamGameMode.h"
#include "TutorialMoveGameMode.generated.h"

/**
 * 
 */
UCLASS()
class GANGNAM_API ATutorialMoveGameMode : public AGangNamGameMode
{
	GENERATED_BODY()

public:


protected:
	virtual void HandleDialogueStep(const FScenarioStep& Step) override;
	
	virtual void BeginPlay() override;

	virtual void HandleGameplayStep(const FScenarioStep& Step) override;
};
