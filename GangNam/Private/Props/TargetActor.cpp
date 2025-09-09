// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/TargetActor.h"
#include "GameModes/GangNamGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATargetActor::ATargetActor()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComponent->SetCollisionObjectType(ECC_WorldDynamic);
	MeshComponent->SetCollisionResponseToAllChannels(ECR_Block); 
	MeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore); 

}


void ATargetActor::Hit()
{
	AGangNamGameMode* MyGameMode = Cast<AGangNamGameMode>(UGameplayStatics::GetGameMode(this));
	if (MyGameMode)
	{
		MyGameMode->OnStepConditionMet();
	}
}

