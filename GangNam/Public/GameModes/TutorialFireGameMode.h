// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameModes/GangNamGameMode.h"
#include "TutorialFireGameMode.generated.h"

/**
 * 
 */
UCLASS()
class GANGNAM_API ATutorialFireGameMode : public AGangNamGameMode
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	virtual void HandleGameplayStep(const FScenarioStep& Step) override;

public:

	
};
