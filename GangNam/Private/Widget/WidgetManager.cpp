// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/WidgetManager.h"
#include "Blueprint/UserWidget.h"
#include "GameModes/GangNamGameMode.h"
#include "Controllers/GangNamPlayerController.h"


UWidgetManager::UWidgetManager()
{
 
}

void UWidgetManager::Init(AGangNamPlayerController* InPC)
{
    PC = InPC;

    if (PC->BottomDialogueWidgetClass)
    {
        BottomDialogueWidgetClass = PC->BottomDialogueWidgetClass;
    }

    if (PC->MiddleDialogueWidgetClass)
    {
        MiddleDialogueWidgetClass = PC->MiddleDialogueWidgetClass;
    }

    if (PC->TopDialogueWidgetClass)
    {
        TopDialogueWidgetClass = PC->TopDialogueWidgetClass;
    }

    if (PC->FadeWidgetClass)
    {
        FadeWidgetClass = PC->FadeWidgetClass;
    }

    if (BottomDialogueWidgetClass)
        BottomDialogueWidget = CreateWidget<UBaseTextWidget>(PC, BottomDialogueWidgetClass);

    if (MiddleDialogueWidgetClass)
        MiddleDialogueWidget = CreateWidget<UBaseTextWidget>(PC, MiddleDialogueWidgetClass);

    if (TopDialogueWidgetClass)
        TopDialogueWidget = CreateWidget<UBaseTextWidget>(PC, TopDialogueWidgetClass);

    if (FadeWidgetClass)
    {
        FadeWidget = CreateWidget<UFadeWidget>(PC, FadeWidgetClass);
        if (FadeWidget)
        {
            FadeWidget->AddToViewport(11);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("FadeWidget null"));
        }
    }
    CurrentDialogueWidget = nullptr;
}



void UWidgetManager::ShowWidget(TSubclassOf<UUserWidget> WidgetClass, int32 ZOrder)
{
    if (!WidgetClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("WidgetManager::ShowWidget - WidgetClass is null"));
        return;
    }

    if (!PC)
    {
        UE_LOG(LogTemp, Error, TEXT("WidgetManager::ShowWidget - Owner PlayerController not found"));
        return;
    }

    if (CurrentWidget)
    {
        RemoveWidget(CurrentWidget);
        CurrentWidget = nullptr;
    }

    CurrentWidget = CreateWidget<UUserWidget>(PC, WidgetClass);
    if (!CurrentWidget)
    {
        UE_LOG(LogTemp, Error, TEXT("WidgetManager::ShowWidget - Failed to create widget instance"));
        return;
    }

    if (!CurrentWidget->IsInViewport())
    {
        CurrentWidget->AddToViewport(ZOrder);
    }
}



void UWidgetManager::RemoveWidget(UUserWidget* WidgetInstance)
{
	if (!WidgetInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("WidgetManager::RemoveWidget - WidgetInstance is null"));
		return;
	}

	WidgetInstance->RemoveFromParent();
    
}




void UWidgetManager::StartDialogue(UDataTable* DialogueTable)
{
    if (!PC || !DialogueTable)
    {
        UE_LOG(LogTemp, Error, TEXT("StartDialogue - Invalid PC, or DataTable"));
        return;
    }

    if (CurrentWidget)
    {
        RemoveWidget(CurrentWidget);
        CurrentWidget = nullptr;
    }

    CurrentDialogueTable = DialogueTable;
    CurrentLineIndex = 0;

    NextDialogue();
}



void UWidgetManager::NextDialogue()
{
    if (!CurrentDialogueTable)
    {
        UE_LOG(LogTemp, Error, TEXT("NextDialogue - DialogueTable is null"));
        return;
    }

    static const FString Context(TEXT("Dialogue Context"));
    TArray<FName> RowNames = CurrentDialogueTable->GetRowNames();

    if (CurrentLineIndex >= RowNames.Num())
    {
        EndDialogue();
        return;
    }

    FDialogueLine* Line = CurrentDialogueTable->FindRow<FDialogueLine>(RowNames[CurrentLineIndex], Context);
    if (!Line)
    {
        EndDialogue();
        return;
    }

    UBaseTextWidget* TargetWidget = nullptr;
    switch (Line->DialogueType)
    {
    case 0: TargetWidget = BottomDialogueWidget; break;
    case 1: TargetWidget = MiddleDialogueWidget; break;
    case 2: TargetWidget = TopDialogueWidget; break;
    }

    if (!TargetWidget)
    {
        UE_LOG(LogTemp, Error, TEXT("NextDialogue - Invalid DialogueType: %d"), Line->DialogueType);
        EndDialogue();
        return;
    }

    if (CurrentDialogueWidget && CurrentDialogueWidget != TargetWidget)
    {
        CurrentDialogueWidget->RemoveFromParent();
    }

    if (!TargetWidget->IsInViewport())
    {
        TargetWidget->AddToViewport(ZORDER_DIALOGUE);
    }

    CurrentDialogueWidget = TargetWidget;
    CurrentDialogueWidget->SetDialogueLine(*Line);
    CurrentLineIndex++;
}





void UWidgetManager::EndDialogue()
{
    if (CurrentDialogueWidget)
    {
        CurrentDialogueWidget->RemoveFromParent();
        CurrentDialogueWidget = nullptr;
    }

    CurrentDialogueTable = nullptr;
    CurrentLineIndex = 0;

    if (UWorld* World = GetWorld())
    {
        if (AGangNamGameMode* GM = Cast<AGangNamGameMode>(World->GetAuthGameMode()))
        {
            GM->OnStepConditionMet();
        }
    }
}


bool UWidgetManager::GetBIsTypingFinished()
{
    if (CurrentDialogueWidget)
    {
        return CurrentDialogueWidget->GetBIsTypingFinished();
    }
    return false;
}


UUserWidget* UWidgetManager::GetCurrentWidget()
{
    return CurrentWidget;
}

void UWidgetManager::PlayFadeIn()
{
    if (FadeWidget)
    {
        if (FadeWidget->FadeIn)
        {
            FadeWidget->PlayAnimation(FadeWidget->FadeIn, 0.f, 1);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Play Fail"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("FadeWidget null"));
    }
}

void UWidgetManager::PlayFadeOut()
{
    if (FadeWidget)
    {
        if (FadeWidget->FadeOut)
        {
            FadeWidget->PlayAnimation(FadeWidget->FadeOut, 0.f, 1);
        }
    }
}