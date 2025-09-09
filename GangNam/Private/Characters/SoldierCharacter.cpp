// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/SoldierCharacter.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/CrowdCharacter.h"
#include "Controllers/SoldierAIController.h"


ASoldierCharacter::ASoldierCharacter()
{
	// custom setting

	PrimaryActorTick.bCanEverTick = true;
	GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;

	AIControllerClass = ASoldierAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ASoldierCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (bIsStation)
	{
		CurrentState = ESoldierState::IdleStation;
	}
}

void ASoldierCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ASoldierCharacter::Shoot()
{
	if (TargetCharacter)
	{
		FVector Dir = (TargetCharacter->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();
		FRotator Rot = Dir.Rotation();
		Rot.Pitch = 0.f;
		Rot.Roll = 0.f;
		SetActorRotation(Rot);

		FRotator FinalRot = GetActorRotation();
		FinalRot.Yaw += 34.13f;
		SetActorRotation(FinalRot);
	}
	Super::Shoot();
}







void ASoldierCharacter::ChaseCharacter()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACrowdCharacter::StaticClass(), FoundActors);

	ACrowdCharacter* ClosestTarget = nullptr;
	float MinDistance = TNumericLimits<float>::Max();

	for (AActor* Actor : FoundActors)
	{
		ACrowdCharacter* Crowd = Cast<ACrowdCharacter>(Actor);
		if (Crowd && Crowd->CurrentState != ECrowdState::Dead)
		{
			float Distance = FVector::Dist(this->GetActorLocation(), Crowd->GetActorLocation());
			if (Distance < MinDistance)
			{
				MinDistance = Distance;
				ClosestTarget = Crowd;
			}
		}
	}

	TargetCharacter = ClosestTarget;

	AAIController* AICon = Cast<AAIController>(GetController());
	if (AICon)
	{
		AICon->MoveToActor(TargetCharacter, 100.f, true, true, true, nullptr, true);
	}
}


void ASoldierCharacter::ChaseLocation()
{

	AAIController* AICon = Cast<AAIController>(GetController());
	if (AICon)
	{
		AICon->MoveToLocation(TargetLocation, 100.f, true, true, false, true, 0, true);
	}
}


void ASoldierCharacter::StopChase()
{
	GetController()->StopMovement();
}


void ASoldierCharacter::SetState(ESoldierState NewState)
{
	if (CurrentState == NewState) return;

	CurrentState = NewState;

	switch (CurrentState)
	{
	case ESoldierState::Idle:
		break;
	case ESoldierState::Shoot:
		if (UAnimInstance* AnimInst = GetMesh()->GetAnimInstance())
		{
			AnimInst->Montage_Stop(0.2f); 
		}
		UpdateHeadLight();
		break;
	}
	
}


void ASoldierCharacter::PlayRandomIdleMontage()
{
	if (CurrentState != ESoldierState::IdleStation || IdleMontages.Num() == 0) return;

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