// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/TutorialMentalGameMode.h"
#include "Widget/SuspectSelectWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Widget/SuspectResultWidget.h"



void ATutorialMentalGameMode::HandleDialogueStep(const FScenarioStep& Step)
{
    Super::HandleDialogueStep(Step);

    if (CurrentStepIndex == 1 && SuspectWidgetClass1)
    {
        if (CurrentWidget)
        {
            CurrentWidget->RemoveFromParent();
            CurrentWidget = nullptr;
        }
        CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), SuspectWidgetClass1);
        if (CurrentWidget)
        {
            CurrentWidget->AddToViewport(10); 
        }
    }

    if (CurrentStepIndex == 3 && SuspectWidgetClass3)
    {
        if (CurrentWidget)
        {
            CurrentWidget->RemoveFromParent();
            CurrentWidget = nullptr;
        }
        CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), SuspectWidgetClass3);
        if (CurrentWidget)
        {
            CurrentWidget->AddToViewport(10);  
            if (USuspectResultWidget* ResultWidget = Cast<USuspectResultWidget>(CurrentWidget))
            {       
                ResultWidget->SetResultImage(SuspectIndex); 
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Now Result Widget"));
        }
    }
}


void ATutorialMentalGameMode::HandleStoryStep(const FScenarioStep& Step)
{
    Super::HandleStoryStep(Step);
    if (CurrentWidget)
    {
        CurrentWidget->RemoveFromParent();
        CurrentWidget = nullptr;
    }
    if (PC && PC->WidgetManager && PC->WidgetManager->GetCurrentWidget())
    {
        UUserWidget* PCWidget = PC->WidgetManager->GetCurrentWidget();

        if (USuspectSelectWidget* SuspectWidget = Cast<USuspectSelectWidget>(PCWidget))
        {
            SuspectWidget->StartCountdown();
        }
    }
}


void ATutorialMentalGameMode::SetSuspectIndex(int32 inValue)
{
    SuspectIndex = inValue;
}