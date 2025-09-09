// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/WeaponCharacter.h"
#include "Components/SpotLightComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Props/TargetActor.h"
#include "Characters/CrowdCharacter.h"

AWeaponCharacter::AWeaponCharacter()
{
 	
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	GetCharacterMovement()->bOrientRotationToMovement = true;

	HeadLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("HeadLight"));
	HeadLight->SetupAttachment(GetMesh(), TEXT("head")); 
	HeadLight->Intensity = 5000.f;
	HeadLight->SetVisibility(false);

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
}


void AWeaponCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (!WeaponMesh || !GetMesh())
	{
		UE_LOG(LogTemp, Error, TEXT("WeaponMesh is nullptr or Mesh is nullptr"));
		return;
	}

	WeaponMesh->AttachToComponent(
		GetMesh(),
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		WeaponSocketName
	);

	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetVisibility(true);
	WeaponMesh->SetRelativeLocation(WeaponRelativeLocation);
	WeaponMesh->SetRelativeRotation(WeaponRelativeRotation);
	WeaponMesh->SetRelativeScale3D(WeaponRelativeScale);
	
}

// Called every frame
void AWeaponCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AWeaponCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AWeaponCharacter::UpdateHeadLight()
{
	if (HeadLight)
	{
		HeadLight->SetVisibility(true);
	}
}

void AWeaponCharacter::Shoot()
{
	if (ShootSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShootSound, GetActorLocation());
	}

	if (ShootMontage && GetMesh() && GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_Play(ShootMontage, 0.5f);
	}

	if (WeaponMesh && MuzzleEffect && WeaponMesh->DoesSocketExist(FName("Muzzle")))
	{
		UGameplayStatics::SpawnEmitterAttached(
			MuzzleEffect,
			WeaponMesh,
			FName("Muzzle"),
			FVector::ZeroVector,
			FRotator(-90.f, 0.f, 0.f),
			EAttachLocation::SnapToTarget,
			true
		);
	}

	FHitResult Hit;
	FVector ShotDir;
	bool bHit = PerformLineTrace(Hit, ShotDir);

	if (bHit && Hit.GetActor())
	{
		// CrowdCharacter
		if (ACrowdCharacter* Crowd = Cast<ACrowdCharacter>(Hit.GetActor()))
		{
			if (BloodEffect)
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(
					GetWorld(),
					BloodEffect,
					Hit.ImpactPoint,
					ShotDir.Rotation()
				);
			}
			Crowd->Hit();
		}
		// TargetActor or Actor
		else
		{
			if (ImpactEffect)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.ImpactPoint, ShotDir.Rotation());
			}
			if (ATargetActor* Target = Cast<ATargetActor>(Hit.GetActor()))
			{
				Target->Hit();
			}
		}
	}
}


bool AWeaponCharacter::PerformLineTrace(FHitResult& Hit, FVector& ShotDirection)
{
	if (!WeaponMesh)
	{
		return false;
	}

	FVector RootLocation = WeaponMesh->GetSocketLocation(FName("root"));
	FVector MuzzleLocation = WeaponMesh->GetSocketLocation(FName("muzzle"));
	FVector Direction = MuzzleLocation - RootLocation;
	Direction.Z = 0.f;
	Direction.Normalize();

	float Range = 5000.f;
	FVector End = MuzzleLocation + Direction * Range;
	ShotDirection = Direction;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, MuzzleLocation, End, ECC_GameTraceChannel1, Params);

	return bHit;
}

