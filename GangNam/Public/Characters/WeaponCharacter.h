// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "GameFramework/Character.h"
#include "WeaponCharacter.generated.h"

UCLASS()
class GANGNAM_API AWeaponCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	FName WeaponSocketName = "RightHandIndex1";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	FVector WeaponRelativeLocation = FVector(2.0f, -17.0f, -7.8f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	FRotator WeaponRelativeRotation = FRotator(-49.6f, 41.2f, 44.8f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	FVector WeaponRelativeScale = FVector(8.0f, 8.0f, 8.0f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	USoundBase* ShootSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "weapon", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* ShootMontage;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* MuzzleEffect;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* ImpactEffect;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UNiagaraSystem* BloodEffect;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float DefaultSpeed = 300.f;

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class USpotLightComponent* HeadLight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soldier")
	bool bLightOn = false;

	void UpdateHeadLight();

	virtual void Shoot();

	bool PerformLineTrace(FHitResult& Hit, FVector& ShotDirection);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	AWeaponCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE UStaticMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
};
