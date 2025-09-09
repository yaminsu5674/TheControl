// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameModes/GangNamGameMode.h"
#include "CargoTruckGameMode.generated.h"

/**
 * 
 */
UCLASS()
class GANGNAM_API ACargoTruckGameMode : public AGangNamGameMode
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
};
