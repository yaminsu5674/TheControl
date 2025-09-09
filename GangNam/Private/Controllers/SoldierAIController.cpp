// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/SoldierAIController.h"
#include "Characters/SoldierCharacter.h"
#include "Navigation/PathFollowingComponent.h"

ASoldierAIController::ASoldierAIController()
{
    PrimaryActorTick.bCanEverTick = false;
    CurrentTarget = FVector::ZeroVector;
}

void ASoldierAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    ControlledSoldier = Cast<ASoldierCharacter>(InPawn);
    if (ControlledSoldier)
    {
        float FirstDelay = FMath::FRandRange(ControlledSoldier->RandomIntMin, ControlledSoldier->RandomIntMax);
        GetWorldTimerManager().SetTimer(MontageTimerHandle, this, &ASoldierAIController::TryPlayIdleMontage, FirstDelay, false);
    }
}

void ASoldierAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
    Super::OnMoveCompleted(RequestID, Result);

    if (!ControlledSoldier) return;

    if (ControlledSoldier->CurrentState == ESoldierState::Shoot)
    {
        IsChasing = false;
        if (Result.IsSuccess())
        {
            ControlledSoldier->Shoot();
        }
    }
}

void ASoldierAIController::TryPlayIdleMontage()
{
    if (!ControlledSoldier) return;

    if (ControlledSoldier->CurrentState == ESoldierState::IdleStation)
    {
        ControlledSoldier->PlayRandomIdleMontage();
    }
    else if (ControlledSoldier->CurrentState == ESoldierState::Shoot)
    {
        ControlledSoldier->ChaseCharacter();
        IsChasing = true;       
    }
    float NextDelay = FMath::FRandRange(ControlledSoldier->RandomIntMin, ControlledSoldier->RandomIntMax);
    GetWorldTimerManager().SetTimer(MontageTimerHandle, this, &ASoldierAIController::TryPlayIdleMontage, NextDelay, false);
}

