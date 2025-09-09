// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/TutorialMoveGameMode.h"




void ATutorialMoveGameMode::HandleDialogueStep(const FScenarioStep& Step)
{
    Super::HandleDialogueStep(Step);

    if (CurrentStepIndex == 2)
    {
        for (ASoldierCharacter* Soldier : SoldierCharacters)
        {
            if (Soldier)
            {
                Soldier->StopChase();
            }
        }
    }
}


void ATutorialMoveGameMode::BeginPlay()
{
    Super::BeginPlay();

    SearchSoldiers();
}



void ATutorialMoveGameMode::HandleGameplayStep(const FScenarioStep& Step)
{
    Super::HandleGameplayStep(Step);

    if (CurrentStepIndex == 1)
    {
        for (ASoldierCharacter* Soldier : SoldierCharacters)
        {
            if (Soldier)
            {
                Soldier->ChaseLocation(); 
            }
        }
    }
    else if (CurrentStepIndex == 3)
    {
        for (ASoldierCharacter* Soldier : SoldierCharacters)
        {
            if (Soldier)
            {
                Soldier->ChaseLocation(); 
            }
        }
    }
}