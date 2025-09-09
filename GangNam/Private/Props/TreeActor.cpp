// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/TreeActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "GameModes/GangNamGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Characters/GangNamCharacter.h"

ATreeActor::ATreeActor()
{
    PrimaryActorTick.bCanEverTick = false;

    TreeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TreeMesh"));
    RootComponent = TreeMesh;

    DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
    DetectionSphere->InitSphereRadius(DetectionRadius);
    DetectionSphere->SetupAttachment(RootComponent);
    DetectionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    DetectionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
    DetectionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ATreeActor::BeginPlay()
{
    Super::BeginPlay();

    DetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &ATreeActor::OnOverlapBegin);

    
}

void ATreeActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this && Cast<AGangNamCharacter>(OtherActor))
    {
        DetectionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        AGangNamGameMode* MyGameMode = Cast<AGangNamGameMode>(UGameplayStatics::GetGameMode(this));
        if (MyGameMode)
        {
           MyGameMode->OnStepConditionMet();   
        }
    }
}