// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/GangNamStationGameMode.h"
#include "Characters/GangNamCharacter.h"
#include "Characters/CrowdCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DirectionalLight.h"
#include "Components/AudioComponent.h"



void AGangNamStationGameMode::BeginPlay()
{
    Super::BeginPlay();

    if (MineWidgetClass)
        MineWidget = CreateWidget<UUserWidget>(PC, MineWidgetClass);

    if (BottomDialogueWidgetClass)
        BottomDialogueWidget = CreateWidget<UBaseImageTextWidget>(PC, BottomDialogueWidgetClass);

    SearchCrowds();
    SearchSoldiers();
}


void AGangNamStationGameMode::HandleDialogueStep(const FScenarioStep& Step)
{
    if (CurrentStepIndex == 1)
    {
        if (ScaryWidgetClass && !ScaryWidget)
        {
            ScaryWidget = CreateWidget<UColorWidget>(GetWorld(), ScaryWidgetClass);
            if (ScaryWidget)
            {
                ScaryWidget->AddToViewport(0);
                ScaryWidget->SetBackColor(0);
            }
        }
        if (PC)
        {
            AGangNamCharacter* GangNamChar = Cast<AGangNamCharacter>(PC->GetPawn());
            if (GangNamChar)
            {
                GangNamChar->StartBulletWidget();
                GangNamChar->MakeBulletFull();
                GangNamChar->bCanInteract = true;
            }
        }
    }
    Super::HandleDialogueStep(Step);
}




void AGangNamStationGameMode::HandleGameplayStep(const FScenarioStep& Step)
{
}


void AGangNamStationGameMode::StartMine(UDataTable* DialogueData, ACrowdCharacter* NewCrowd)
{
    AGangNamCharacter* Character = nullptr;
    NowCrowd = NewCrowd;
    if (PC)
    {
        if (APawn* Pawn = PC->GetPawn())
        {
            Character = Cast<AGangNamCharacter>(Pawn);
            if (Character)
            {
                Character->SetIsInputEnabled(false);
            }
        }
    }
    if (DialogueData)
    {
        IsMine = true;
        CurrentDialogueTable = DialogueData;
        if (MineWidget)
        {
            MineWidget->AddToViewport(2);
        }
    }
}


void AGangNamStationGameMode::StartAsk()
{
    IsAsk = true;
    IsMine = false;
    if (MineWidget)
    {
        MineWidget->RemoveFromParent();
    }
    if (AskWidgetClass && PC)
    {
        AskWidget = CreateWidget<UAskWidget>(PC, AskWidgetClass);
        if (AskWidget)
        {
            AskWidget->AddToViewport(2);
        }
    }
}


void AGangNamStationGameMode::StartBottom()
{
    if (!IsAsk) return;

    IsBottom = true;
    IsAsk = false;
    Index = AskWidget->NowIndex;

    if (AskWidget)
    {
        AskWidget->NowIndex = 0;
        AskWidget->RemoveFromParent();
        AskWidget = nullptr;
    }

    if (BottomDialogueWidget)
    {
        BottomDialogueWidget->AddToViewport(2);
    }
    if (!CurrentDialogueTable)
    {
        UE_LOG(LogTemp, Error, TEXT("StartBottom - CurrentDialogueTable is null"));
        return;
    }

    static const FString Context(TEXT("StartBottom Context"));
    TArray<FName> RowNames = CurrentDialogueTable->GetRowNames();

    if (Index < 0 || Index >= RowNames.Num())
    {
        UE_LOG(LogTemp, Error, TEXT("StartBottom - Invalid index: %d (Row count: %d)"), Index, RowNames.Num());
        return;
    }

    FDialogueLine* Line = CurrentDialogueTable->FindRow<FDialogueLine>(RowNames[Index], Context);
    if (!Line)
    {
        UE_LOG(LogTemp, Error, TEXT("StartBottom - Row not found at index %d"), Index);
        return;
    }

    if (BottomDialogueWidget)
    {
        BottomDialogueWidget->SetDialogueLine(*Line);
    }
}

void AGangNamStationGameMode::StartKill()
{
    // auto aim shot
    AGangNamCharacter* Shooter = Cast<AGangNamCharacter>(PC->GetPawn());
 
    if (Shooter && NowCrowd)
    {
        Shooter->ReadyAuto();

        if (!Shooter || !NowCrowd) return;

        FVector MuzzleLocation = Shooter->GetWeaponMesh()->GetSocketLocation(FName("Muzzle"));
        FVector TargetLocation = NowCrowd->GetActorLocation();
        FVector CharacterLocation = Shooter->GetActorLocation();

        FVector DirToTarget = (TargetLocation - CharacterLocation).GetSafeNormal2D();
        FRotator TargetRotation = DirToTarget.Rotation();
        TargetRotation.Pitch = 0.f;
        TargetRotation.Roll = 0.f;
        Shooter->SetActorRotation(TargetRotation);

        FRotator FinalRotation = Shooter->GetActorRotation();
        FinalRotation.Yaw += 34.13f;
        Shooter->SetActorRotation(FinalRotation);

        FVector Backward = -Shooter->GetActorForwardVector(); 
        FVector NewLocation = Shooter->GetActorLocation() + Backward * 30.f; 
        Shooter->SetActorLocation(NewLocation);
        
        FTimerHandle DelayHandle;
        Shooter->GetWorldTimerManager().SetTimer(
            DelayHandle,
            [Shooter, this]()
            {
                Shooter->ShootAuto();
                AGangNamCharacter* Character = nullptr;
                if (PC)
                {
                    if (APawn* Pawn = PC->GetPawn())
                    {
                        Character = Cast<AGangNamCharacter>(Pawn);
                        if (Character)
                        {
                            Character->SetIsInputEnabled(true);
                        }
                    }
                }
                FTimerHandle PointScaryHandle;
                Shooter->GetWorldTimerManager().SetTimer(
                    PointScaryHandle,
                    [this]()
                    {
                        PointScary(100);
                    },
                    1.0f, 
                    false
                );
            },
            1.0f, false
        );

    }
    // auto aim end
}

void AGangNamStationGameMode::EndDialogue()
{
    DialogueCount++;
    if (NowCrowd)
    {
        NowCrowd->BlockInteract();
    }

    AGangNamCharacter* Character = nullptr;
    if (PC)
    {
        if (APawn* Pawn = PC->GetPawn())
        {
            Character = Cast<AGangNamCharacter>(Pawn);
            if (Character)
            {
                Character->SetIsInputEnabled(true);
            }
        }
    }
    if (BottomDialogueWidget)
    {
        BottomDialogueWidget->RemoveFromParent();
    }
    CurrentDialogueTable = nullptr;
    IsBottom = false;

    // scary logic
    if (Index == 3)
    {
        IsBottom = false;
        StartKill();
        return;
    }
    else
    {
        if (Index == 0)
        {
            PointScary(5);
        }
        if (Index == 1)
        {
            PointScary(10);
        }
        if (Index == 2)
        {
            PointScary(30);
        }
    }
}


void AGangNamStationGameMode::PointScary(int8 inValue)
{
    ScaryPoint += inValue;
   
    if (ScaryWidget)
    {
        if(ScaryPoint >= 100)
        {
            ScaryWidget->SetBackColor(3);
        }
        else if (ScaryPoint >= 60)
        {
            ScaryWidget->SetBackColor(2);
        }
        else if (ScaryPoint >= 20)
        {
            ScaryWidget->SetBackColor(1);
        }

    }

    if (DialogueCount == 6 && ScaryPoint < 100)
    {
        OnStepConditionMet();
    }
    

    if (ScaryPoint >= 100)
    {
        IsScary = true;
         
        if (ChangeWidgetClass)
        {
            ChangeWidget = CreateWidget<UUserWidget>(PC, ChangeWidgetClass);
            if (ChangeWidget)
            {
                ChangeWidget->AddToViewport(5);
            }
        }
            
    }
}

void AGangNamStationGameMode::SearchCrowds()
{
    CrowdCharacters.Empty(); 

    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(this, ACrowdCharacter::StaticClass(), FoundActors);

    for (AActor* Actor : FoundActors)
    {
        if (ACrowdCharacter* Crowd = Cast<ACrowdCharacter>(Actor))
        {
            CrowdCharacters.Add(Crowd);
            CrowdCount++;
        }
    }
}

void AGangNamStationGameMode::CountCrowd()
{
    CrowdCount--;
    if (!IsScary)
    {
        PointScary(100);
    }
    if (CrowdCount == 0)
    {
        // end game logic
        OnStepConditionMet();
    }
}


void AGangNamStationGameMode::ChangeLogic()
{
    if (PC)
    {
        AGangNamCharacter* GangNamChar = Cast<AGangNamCharacter>(PC->GetPawn());
        if (GangNamChar)
        {
            GangNamChar->UpdateHeadLight();
        }
    }

    for (ASoldierCharacter* Soldier : SoldierCharacters)
    {
        if (Soldier)
        {
            Soldier->SetState(ESoldierState::Shoot);
        }
    }
    for (ACrowdCharacter* Crowd : CrowdCharacters)
    {
        if (Crowd)
        {
            Crowd->BlockInteract();
            Crowd->SetState(ECrowdState::Confused);
            Crowd->ChaseLocation();
        }
    }
    TArray<AActor*> FoundLights;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADirectionalLight::StaticClass(), FoundLights);

    for (AActor* Actor : FoundLights)
    {
        ADirectionalLight* DirLight = Cast<ADirectionalLight>(Actor);
        if (DirLight)
        {
            DirLight->SetBrightness(ScaryIntensity); 
            DirLight->SetLightColor(FLinearColor(50.f / 255.f, 80.f / 255.f, 120.f / 255.f)); 
        }
    }

    if (SoundList.IsValidIndex(1) && SoundList[1] && BGMComponent)
    {
        if (BGMComponent->IsPlaying())
        {
            BGMComponent->FadeOut(1.f, 0.f); 
        }
        FTimerHandle TimerHandle;
        GetWorldTimerManager().SetTimer(
            TimerHandle,
            [this]()
            {
                if (SoundList.IsValidIndex(1) && SoundList[1] && BGMComponent)
                {
                    BGMComponent->SetSound(SoundList[1]);
                    BGMComponent->FadeIn(2.f, 1.f, 0.f); 
                    UE_LOG(LogTemp, Warning, TEXT("Scary BGM Started!"));
                }
            },
            1.f, 
            false
        );
    }
}


bool AGangNamStationGameMode::GetIsBottomTypingFinished()
{
    if (BottomDialogueWidget)
    {
        return BottomDialogueWidget->GetBIsTypingFinished();
    }
    return false;
}