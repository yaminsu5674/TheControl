// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameModes/GangNamGameMode.h"
#include "GateWayGameMode.generated.h"

/**
 * 
 */
UCLASS()
class GANGNAM_API AGateWayGameMode : public AGangNamGameMode
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	virtual void HandleStoryStep(const FScenarioStep& Step) override;

public:

	
};
