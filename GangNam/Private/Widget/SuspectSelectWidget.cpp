// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/SuspectSelectWidget.h"
#include "Components/TextBlock.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameModes/GangNamGameMode.h"
#include "Kismet/GameplayStatics.h"


void USuspectSelectWidget::StartCountdown(int32 InSeconds)
{
    RemainingSeconds = InSeconds;

    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(CountdownHandle);
        GetWorld()->GetTimerManager().SetTimer(
            CountdownHandle,
            this,
            &USuspectSelectWidget::TickCountdown,
            1.f,
            true
        );
    }
    if (CountdownLabel)
    {
        CountdownLabel->SetText(FText::AsNumber(RemainingSeconds));
    }
}

void USuspectSelectWidget::TickCountdown()
{
    RemainingSeconds--;

    if (RemainingSeconds < 0)
    {
        if (GetWorld())
        {
            GetWorld()->GetTimerManager().ClearTimer(CountdownHandle);
        }

        if (AGameModeBase* GM = UGameplayStatics::GetGameMode(this))
        {
            if (AGangNamGameMode* GangNamGM = Cast<AGangNamGameMode>(GM))
            {
                GangNamGM->OnStepConditionMet();
            }
        }
        OnSelectionTimeout.Broadcast();
        return;
    }

    if (CountdownLabel)
    {
        CountdownLabel->SetText(FText::AsNumber(RemainingSeconds));
    }
}

void USuspectSelectWidget::MoveLeft()
{
    NowIndex = (NowIndex - 1 + 3) % 3;
}

void USuspectSelectWidget::MoveRight()
{
    NowIndex = (NowIndex + 1) % 3;
}
