// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/GateWayGameMode.h"




void AGateWayGameMode::BeginPlay()
{
    Super::BeginPlay();
    SearchSoldiers();
}


void AGateWayGameMode::HandleStoryStep(const FScenarioStep& Step)
{
    Super::HandleStoryStep(Step);

    if (CurrentStepIndex == 1)
    {
        for (ASoldierCharacter* Soldier : SoldierCharacters)
        {
            if (Soldier)
            {
                Soldier->ChaseLocation();
            }
        }
        FTimerHandle TimerHandle;
        GetWorld()->GetTimerManager().SetTimer(
            TimerHandle,
            this,
            &AGateWayGameMode::OnStepConditionMet, 
            5.0f, 
            false 
        );
    }
    if (CurrentStepIndex == 3)
    {
        for (ASoldierCharacter* Soldier : SoldierCharacters)
        {
            if (Soldier)
            {
                Soldier->TargetLocation = FVector(435.0f, 215.0f, -200.0f);
                Soldier->ChaseLocation();
            }
        }
        FTimerHandle TimerHandle;
        GetWorld()->GetTimerManager().SetTimer(
            TimerHandle,
            this,
            &AGateWayGameMode::OnStepConditionMet, 
            5.0f, 
            false 
        );
    }
}