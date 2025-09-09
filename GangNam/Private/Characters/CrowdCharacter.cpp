// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CrowdCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameModes/GangNamStationGameMode.h"
#include "Controllers/CrowdAIController.h"

ACrowdCharacter::ACrowdCharacter()
{
    GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
    GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
    GetCharacterMovement()->bUseRVOAvoidance = true;   
    GetCharacterMovement()->bUseSeparateBrakingFriction = true;
    GetCharacterMovement()->bEnablePhysicsInteraction = false; 
    GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
    

    AIControllerClass = ACrowdAIController::StaticClass();
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

    PrimaryActorTick.bCanEverTick = true;
    CurrentState = ECrowdState::Idle;

    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetCapsuleComponent()->SetCollisionObjectType(ECC_Pawn);
    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Block);
    GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore); 

    GetCharacterMovement()->MaxWalkSpeed = 200.f;
    GetCharacterMovement()->MaxWalkSpeedCrouched = 150.f;
    GetCharacterMovement()->bOrientRotationToMovement = true;
    bUseControllerRotationYaw = false;

    InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
    InteractionSphere->SetupAttachment(RootComponent);
    InteractionSphere->InitSphereRadius(50.f);
    InteractionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    InteractionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
    InteractionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

    QuestionMarkWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("QuestionMarkWidget"));
    QuestionMarkWidget->SetupAttachment(RootComponent);
    QuestionMarkWidget->SetRelativeLocation(FVector(0.f, 0.f, 200.f)); 
    QuestionMarkWidget->SetWidgetSpace(EWidgetSpace::Screen);
    QuestionMarkWidget->SetDrawAtDesiredSize(true);
}

void ACrowdCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (InteractionSphere)
    {
        InteractionSphere->OnComponentBeginOverlap.AddDynamic(this, &ACrowdCharacter::OnOverlapBegin);
       
    }


    float RandomSpeed = FMath::FRandRange(300.f, 500.f);
    GetCharacterMovement()->MaxWalkSpeed = RandomSpeed;


    if (QuestionMarkWidget)
    {
        if (bInteractable)
        {
            QuestionMarkWidget->SetVisibility(true);
        }
        else
        {
            QuestionMarkWidget->SetVisibility(false);
        }
    }

    TArray<FVector> PossibleLocations = {
    FVector(-120.f,  -1420.f, 100.f),
    FVector(1150.f,  -1485.f, 100.f),
    FVector(2220.f,  -1485.f, 100.f),
    FVector(2220.f,  -3060.f, 100.f),
    FVector(925.f,   -3060.f, 100.f),
    FVector(-290.f,  -3060.f, 100.f)
    };

    if (PossibleLocations.Num() > 0)
    {
        int32 RandomIndex = FMath::RandRange(0, PossibleLocations.Num() - 1);
        TargetLocation = PossibleLocations[RandomIndex];
    }

}

void ACrowdCharacter::Hit()
{
    BlockInteract();
    if (!bIsDead)
    {
        bIsDead = true;
        
        GetCharacterMovement()->DisableMovement();
        GetCharacterMovement()->SetComponentTickEnabled(false);
        GetCharacterMovement()->bComponentShouldUpdatePhysicsVolume = false;

        GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        GetCapsuleComponent()->SetCanEverAffectNavigation(false); 
        
        
        SetState(ECrowdState::Dead);
        GetController()->StopMovement();
        
        AGameModeBase* GMBase = UGameplayStatics::GetGameMode(GetWorld());
        if (!GMBase) return;
        
        AGangNamStationGameMode* GM = Cast<AGangNamStationGameMode>(GMBase);
        if (!GM) return;

        GM->CountCrowd();
   }
}

void ACrowdCharacter::Interact()
{
    if (bPlayerInRange)
    {
        if (bInteractable && DialogueData)
        {
            UE_LOG(LogTemp, Warning, TEXT("Interact with NPC -> Dialogue triggered!"));

            if (AGameModeBase* GM = UGameplayStatics::GetGameMode(this))
            {
                if (AGangNamStationGameMode* StationGM = Cast<AGangNamStationGameMode>(GM))
                {
                    StationGM->StartMine(DialogueData, this);
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("Failed to cast GameMode to GangNamStationGameMode"));
                }
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("This NPC has no dialogue."));
        }
    }
}

void ACrowdCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this)
    {
        AGangNamCharacter* Player = Cast<AGangNamCharacter>(OtherActor);
        if (Player)
        {
            bPlayerInRange = true;
            CachedPlayer = Player;
            Player->CurrentInteractTarget = this;
        }
    }
}



void ACrowdCharacter::SetState(ECrowdState NewState)
{
    if (CurrentState == NewState || CurrentState == ECrowdState::Dead) return;

    CurrentState = NewState;

    switch (CurrentState)
    {
    case ECrowdState::Idle:
        break;

    case ECrowdState::Confused:
        if (UAnimInstance* AnimInst = GetMesh()->GetAnimInstance())
        {
            AnimInst->Montage_Stop(0.2f);
        }
        break;

    case ECrowdState::Dead:
        if (UAnimInstance* AnimInst = GetMesh()->GetAnimInstance())
        {
            AnimInst->Montage_Stop(0.2f);

            if (DeathMontages.Num() > 0)
            {
                int32 Index = FMath::RandRange(0, DeathMontages.Num() - 1);
                UAnimMontage* MontageToPlay = DeathMontages[Index];
                if (MontageToPlay)
                {
                    if (GetMesh())
                    {
                        GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
                    }

                    AnimInst->Montage_Play(MontageToPlay, 1.f);

                    FOnMontageEnded EndDelegate;
                    EndDelegate.BindUObject(this, &ACrowdCharacter::OnDeathMontageEnded);
                    AnimInst->Montage_SetEndDelegate(EndDelegate, MontageToPlay);
                }
            }
        }
        break;
    }
}



void ACrowdCharacter::ChaseLocation()
{
    if (CurrentState == ECrowdState::Dead)
        return;
    AAIController* AICon = Cast<AAIController>(GetController());
    if (AICon)
    {
        AICon->MoveToLocation(TargetLocation, 100.f, true, true, false, true, 0, true);
    }
}


void ACrowdCharacter::StopChase()
{
    GetController()->StopMovement();
}


void ACrowdCharacter::PlayRandomIdleMontage()
{
    if (CurrentState != ECrowdState::Idle || IdleMontages.Num() == 0) return;

    int32 Index = FMath::RandRange(0, IdleMontages.Num() - 1);
    UAnimMontage* MontageToPlay = IdleMontages[Index];

    if (MontageToPlay)
    {
        if (UAnimInstance* AnimInst = GetMesh()->GetAnimInstance())
        {
            AnimInst->Montage_Play(MontageToPlay);
        }
    }
}

void ACrowdCharacter::PlayRandomConfusedMontage()
{
    if (CurrentState != ECrowdState::Confused || ConfusedMontages.Num() == 0) return;

    int32 Index = FMath::RandRange(0, ConfusedMontages.Num() - 1);
    UAnimMontage* MontageToPlay = ConfusedMontages[Index];

    if (MontageToPlay)
    {
        if (UAnimInstance* AnimInst = GetMesh()->GetAnimInstance())
        {
            AnimInst->Montage_Play(MontageToPlay);
        }
    }
}

void ACrowdCharacter::BlockInteract()
{
    bInteractable = false;
    if (QuestionMarkWidget)
    {
        QuestionMarkWidget->SetVisibility(false);
    }
    if (InteractionSphere)
    {
        InteractionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        InteractionSphere->SetGenerateOverlapEvents(false);
    }

}


void ACrowdCharacter::OnDeathMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    if (GetMesh())
    {
        GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;
        GetMesh()->bPauseAnims = true;
    }
}