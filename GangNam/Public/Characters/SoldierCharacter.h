// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/MoveInterface.h"
#include "Characters/WeaponCharacter.h"
#include "SoldierCharacter.generated.h"

UENUM(BlueprintType)
enum class ESoldierState : uint8
{
	Idle,
	IdleStation,
	Shoot,
};

UCLASS()
class GANGNAM_API ASoldierCharacter : public AWeaponCharacter, public IMoveInterface
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	ASoldierCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	ESoldierState CurrentState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crowd")
	float RandomIntMin = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crowd")
	float RandomIntMax = 4.f;

	void SetState(ESoldierState NewState);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	TArray<UAnimMontage*> IdleMontages;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soldier")
	bool bIsTutorial = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soldier")
	bool bIsStation = false;

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	ACharacter* TargetCharacter = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	FVector TargetLocation;

	virtual void Shoot() override;

	void ChaseCharacter();


	virtual void PlayRandomIdleMontage() override;

	virtual void ChaseLocation() override;

	virtual void StopChase() override;
};