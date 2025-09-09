// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controllers/GangNamAIController.h"
#include "SoldierAIController.generated.h"

class ASoldierCharacter;
/**
 * 
 */
UCLASS()
class GANGNAM_API ASoldierAIController : public AGangNamAIController
{
	GENERATED_BODY()

public:
    ASoldierAIController();

    virtual void OnPossess(APawn* InPawn) override;

    virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

private:
    UPROPERTY()
    ASoldierCharacter* ControlledSoldier;

    UPROPERTY()
    FVector CurrentTarget;

    FTimerHandle MontageTimerHandle;

    UPROPERTY()
    bool IsChasing = false;

    void TryPlayIdleMontage();
	
};
