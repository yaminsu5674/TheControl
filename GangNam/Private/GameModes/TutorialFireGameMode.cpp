// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/TutorialFireGameMode.h"
#include "Characters/GangNamCharacter.h"


void ATutorialFireGameMode::BeginPlay()
{
    Super::BeginPlay();

    if (PC)
    {
        if (APawn* Pawn = PC->GetPawn())
        {
            if (AGangNamCharacter* MyCharacter = Cast<AGangNamCharacter>(Pawn))
            {
                MyCharacter->StartBulletWidget();
            }
        }
    }
}



void ATutorialFireGameMode::HandleGameplayStep(const FScenarioStep& Step)
{
    Super::HandleGameplayStep(Step);

    AGangNamCharacter* Character = nullptr;
    if (PC)
    {
        if (APawn* Pawn = PC->GetPawn())
        {
            Character = Cast<AGangNamCharacter>(Pawn);
            if (Character)
            {
                Character->bCanMove = false;
            }
        }
    }
}