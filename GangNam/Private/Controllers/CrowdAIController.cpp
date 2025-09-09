// Fill out your copyright notice in the Description page of Project Settings.

#include "Controllers/CrowdAIController.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Navigation/PathFollowingComponent.h"

ACrowdAIController::ACrowdAIController()
{
    PrimaryActorTick.bCanEverTick = false;
    CurrentTarget = FVector::ZeroVector;
}

void ACrowdAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    ControlledCrowd = Cast<ACrowdCharacter>(InPawn);
    if (ControlledCrowd)
    {
        float FirstDelay = FMath::FRandRange(ControlledCrowd->RandomIntMin, ControlledCrowd->RandomIntMax);
        GetWorldTimerManager().SetTimer(MontageTimerHandle, this, &ACrowdAIController::TryPlayIdleMontage, FirstDelay, false);
    }
}

void ACrowdAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
    Super::OnMoveCompleted(RequestID, Result);

    if (!ControlledCrowd) return;

    if (ControlledCrowd->CurrentState == ECrowdState::Confused)
    {
        if (Result.IsSuccess())
        {
            IsArrived = true;

            float Delay = FMath::FRandRange(ControlledCrowd->RandomIntMin, ControlledCrowd->RandomIntMax);
            GetWorldTimerManager().SetTimer(
                MontageTimerHandle,
                this,
                &ACrowdAIController::TryPlayIdleMontage,
                Delay,
                false
            );
        }
        else
        {
            float Delay = FMath::FRandRange(ControlledCrowd->RandomIntMin, ControlledCrowd->RandomIntMax);
            GetWorldTimerManager().SetTimer(
                MontageTimerHandle,
                this,
                &ACrowdAIController::TryPlayIdleMontage,
                Delay,
                false
            );
        }
    }
}

void ACrowdAIController::TryPlayIdleMontage()
{
    if (!ControlledCrowd) return;

    if (ControlledCrowd->CurrentState == ECrowdState::Idle)
    {
        ControlledCrowd->PlayRandomIdleMontage();
    }
    else if (ControlledCrowd->CurrentState == ECrowdState::Confused && IsArrived)
    {
        ControlledCrowd->PlayRandomConfusedMontage();
    }

    float NextDelay = FMath::FRandRange(ControlledCrowd->RandomIntMin, ControlledCrowd->RandomIntMax);
    GetWorldTimerManager().SetTimer(MontageTimerHandle, this, &ACrowdAIController::TryPlayIdleMontage, NextDelay, false);
}

