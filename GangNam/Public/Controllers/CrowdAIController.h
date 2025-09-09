// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Characters/CrowdCharacter.h"
#include "CrowdAIController.generated.h"


UCLASS()
class ACrowdAIController : public AAIController
{
    GENERATED_BODY()

public:
    ACrowdAIController();

    virtual void OnPossess(APawn* InPawn) override;

    virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

private:
    UPROPERTY()
    ACrowdCharacter* ControlledCrowd;

    UPROPERTY()
    FVector CurrentTarget;

    FTimerHandle MontageTimerHandle;

    UPROPERTY()
    bool IsArrived = false;

    void TryPlayIdleMontage();

};
