// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameModes/GangNamGameMode.h"
#include "Controllers/GangNamPlayerController.h"
#include "Characters/GangNamCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

AGangNamGameMode::AGangNamGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AGangNamPlayerController::StaticClass();
	DefaultPawnClass = nullptr;

    BGMComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("BGMComponent"));
    BGMComponent->bAutoActivate = false;
    BGMComponent->bIsUISound = true; 

    SFXComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("SFXComponent"));
    SFXComponent->bAutoActivate = false;
    SFXComponent->bIsUISound = true;
}

void AGangNamGameMode::BeginPlay()
{
    Super::BeginPlay();

    PC = Cast<AGangNamPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
    if (PC && PC->WidgetManager)
    {
        PC->WidgetManager->Init(PC);
        PC->WidgetManager->PlayFadeIn();
    }

    CurrentStepIndex = 0;

    if (ScenarioDataTable)
    {
        GoToNextStep();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Scenario No!!!!"));
    }
    if (SoundList.IsValidIndex(0) && SoundList[0])
    {
        BGMComponent->SetSound(SoundList[0]);
        BGMComponent->FadeIn(5.f, 1.f, 0.f);
    }
}

void AGangNamGameMode::GoToNextStep()
{
    static const FString Context(TEXT("Scenario Context"));
    
    TArray<FName> RowNames = ScenarioDataTable->GetRowNames();
    if (CurrentStepIndex >= RowNames.Num())
    {
        UE_LOG(LogTemp, Warning, TEXT("Scenario Finished"));
        return;
    }

    FScenarioStep* StepRow = ScenarioDataTable->FindRow<FScenarioStep>(RowNames[CurrentStepIndex], Context);
    if (StepRow)
    {
        ExecuteStep(*StepRow);
    }
}



void AGangNamGameMode::ExecuteStep(const FScenarioStep& Step)
{
    AGangNamCharacter* Character = nullptr;
    if (PC)
    {
        if (APawn* Pawn = PC->GetPawn())
        {
            Character = Cast<AGangNamCharacter>(Pawn);
        }
    }

    switch (Step.StepType)
    {
    case EScenarioStepType::Story:
        if (Character)
        {
            Character->SetIsInputEnabled(false);
        }
        HandleStoryStep(Step);
        break;
    case EScenarioStepType::Dialogue:
        if (Character)
        {
            Character->SetIsInputEnabled(false);
        }
        HandleDialogueStep(Step);
        break;
    case EScenarioStepType::Gameplay:
        if (Character)
        {
            Character->SetIsInputEnabled(true);
        }
        HandleGameplayStep(Step);
        break;
    case EScenarioStepType::Cinematic:
        HandleCinematicStep(Step);
        break;
    case EScenarioStepType::NextLevel:
        HandleLevelTransitionStep(Step);
        break;
    default:
        break;
    }
}



void AGangNamGameMode::OnStepConditionMet()
{
    CurrentStepIndex++;
    GoToNextStep();
}

void AGangNamGameMode::OnNextDialogue()
{
    if (PC && PC->WidgetManager)
    {
        PC->WidgetManager->NextDialogue();
    }
}



void AGangNamGameMode::HandleStoryStep(const FScenarioStep& Step)
{
    TSubclassOf<UUserWidget> WidgetClass = Step.WidgetClass.LoadSynchronous();
    if (PC && WidgetClass)
    {
        PC->ShowWidget(WidgetClass, UWidgetManager::ZORDER_STORY);       
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("ShowWidget not called"));
    }
}



void AGangNamGameMode::HandleDialogueStep(const FScenarioStep& Step)
{
    if (!PC || !PC->WidgetManager)
    {
        UE_LOG(LogTemp, Error, TEXT("HandleDialogueStep - PC or WidgetManager is null"));
        return;
    }
    UDataTable* DialogueTable = Step.DialogueDataTable.LoadSynchronous();
    if (!DialogueTable)
    {
        UE_LOG(LogTemp, Error, TEXT("HandleDialogueStep - Failed to load DialogueDataTable"));
        return;
    }

    PC->WidgetManager->StartDialogue(DialogueTable);
}



void AGangNamGameMode::HandleGameplayStep(const FScenarioStep& Step)
{

}



void AGangNamGameMode::HandleCinematicStep(const FScenarioStep& Step)
{

}



void AGangNamGameMode::HandleLevelTransitionStep(const FScenarioStep& Step)
{
    if (PC)
    {
        PC->WidgetManager->PlayFadeOut();
    }
    if (BGMComponent && BGMComponent->IsPlaying())
    {
        BGMComponent->FadeOut(5.f, 0.f);
    }

    if (!Step.NextLevelName.IsNone())
    {
        GetWorldTimerManager().SetTimerForNextTick([this, LevelName = Step.NextLevelName]()
            {
                FTimerHandle TempHandle;
                GetWorldTimerManager().SetTimer(TempHandle, [this, LevelName]()
                    {
                        UGameplayStatics::OpenLevel(this, LevelName);
                    }, 5.f, false);
            });
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("No NextLevel's Name!!"));
    }
}

void AGangNamGameMode::SearchSoldiers()
{
    SoldierCharacters.Empty(); 

    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(this, ASoldierCharacter::StaticClass(), FoundActors);

    for (AActor* Actor : FoundActors)
    {
        if (ASoldierCharacter* Soldier = Cast<ASoldierCharacter>(Actor))
        {
            SoldierCharacters.Add(Soldier);
        }
    }
}

void AGangNamGameMode::PlayEffectSound(int32 Index)
{
    if (SoundList.IsValidIndex(Index) && SoundList[Index])
    {
        SFXComponent->SetSound(SoundList[Index]);
        SFXComponent->Play();
    }
}