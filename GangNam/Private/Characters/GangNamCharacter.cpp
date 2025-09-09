// Copyright Epic Games, Inc. All Rights Reserved.

#include "Characters/GangNamCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/World.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "InputMappingContext.h" 
#include "Blueprint/UserWidget.h"
#include "GameModes/TutorialFireGameMode.h"
#include "Characters/CrowdCharacter.h"


DEFINE_LOG_CATEGORY(LogTemplateCharacter);



AGangNamCharacter::AGangNamCharacter()
{
	// Custom setting
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	AutoPossessAI = EAutoPossessAI::Disabled;
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
	GetCharacterMovement()->bEnablePhysicsInteraction = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	GetCharacterMovement()->bPushForceScaledToMass = false;

	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 1000.f;
	CameraBoom->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false;

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}


//////////////////////////////////////////////////////////////////////////
// Input

void AGangNamCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AGangNamCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGangNamCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGangNamCharacter::Look);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &AGangNamCharacter::Interact);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &AGangNamCharacter::ShootInput);
		EnhancedInputComponent->BindAction(ReadyAction, ETriggerEvent::Started, this, &AGangNamCharacter::Ready);
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &AGangNamCharacter::Reload);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AGangNamCharacter::BeginPlay()
{
	Super::BeginPlay();

	

	// Camera Boom setting
	if (!IsPerspective)
	{
		TopDownCameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
		TopDownCameraComponent->OrthoWidth = 800.f;

		if (AGangNamGameMode* GM = Cast<AGangNamGameMode>(UGameplayStatics::GetGameMode(this)))
		{
			if (TopDownCameraComponent)
			{
				TopDownCameraComponent->OrthoWidth = GM->ModeCameraOrthoWidth;
			}
		}
	}
	else
	{
		
		TopDownCameraComponent->ProjectionMode = ECameraProjectionMode::Perspective;
		TopDownCameraComponent->FieldOfView = 70.f;
		CameraBoom->SocketOffset = FVector(0.f, 400.f, 200.f); 
	}
}



void AGangNamCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}



void AGangNamCharacter::Move(const FInputActionValue& Value)
{
	if (!bCanMove) return;

	FVector2D MovementVector = Value.Get<FVector2D>();
	if (Controller == nullptr || CameraBoom == nullptr) return;
	FRotator BoomRotation = CameraBoom->GetComponentRotation();

	FRotator YawRotation(0, BoomRotation.Yaw, 0);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}


void AGangNamCharacter::Look(const FInputActionValue& Value)
{
	if (IsUpperBodyReady)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		if (!PC) return;

		FVector WorldLocation, WorldDirection;

		if (PC->DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
		{
			FVector Start = WorldLocation;
			FVector End = Start + WorldDirection * 10000.f;
			FHitResult HitResult;
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(this);

			if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
			{
				FVector Target = HitResult.ImpactPoint;
				FVector MyLocation = GetActorLocation();
				FVector Direction = (Target - MyLocation);
				Direction.Z = 0.f;

				if (!Direction.IsNearlyZero())
				{
					FRotator NewRotation = Direction.Rotation();
					SetActorRotation(NewRotation);
				}
			}
		}
	}
	else
	{
		FVector2D LookAxisVector = Value.Get<FVector2D>();

		if (Controller != nullptr)
		{
			AddControllerYawInput(LookAxisVector.X);
			AddControllerPitchInput(LookAxisVector.Y);
		}
	}

	
}


void AGangNamCharacter::Interact(const FInputActionValue& Value)
{
	if (!bCanInteract)
		return;
	if (CurrentInteractTarget)
	{
		CurrentInteractTarget->Interact();
	}

}



void AGangNamCharacter::ShootInput(const FInputActionValue& Value)
{
	Shoot();
}


void AGangNamCharacter::Shoot()
{
	if (!bCanShoot || !IsUpperBodyReady || CurrentBullet <= 0) return;
	
	Super::Shoot();

	CurrentBullet--;

	if (BulletWidget)
	{
		switch (CurrentBullet)
		{
		case 20: BulletWidget->SetBackColor(1); break;
		case 10: BulletWidget->SetBackColor(2); break;
		case 0:  BulletWidget->SetBackColor(3); break;
		default: break;
		}
	}
}





void AGangNamCharacter::ShootAuto()
{
	Shoot();
}



void AGangNamCharacter::Ready(const FInputActionValue& Value)
{
	if (!bCanShoot) return;


	IsUpperBodyReady = !IsUpperBodyReady;
	if (IsUpperBodyReady)
	{
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
	else
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
	
}

void AGangNamCharacter::ReadyAuto()
{
	IsUpperBodyReady = true;
	if (IsUpperBodyReady)
	{
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
}


void AGangNamCharacter::Reload(const FInputActionValue& Value)
{
	if (!bCanShoot) return;

	CurrentBullet = MaxBullet;
	if (BulletWidget)
	{
		BulletWidget->SetBackColor(0);
	}

	if (ReloadMontage && GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_Play(ReloadMontage, 1.f);
	}

	if (ATutorialFireGameMode* GM = Cast<ATutorialFireGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		if (GM->CurrentStepIndex == 1)
		{
			GM->OnStepConditionMet();
		}
	}
}





void AGangNamCharacter::SetIsInputEnabled(bool bEnable)
{
	IsInputEnabled = bEnable;

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC) return;

	if (IsInputEnabled)
	{
		FInputModeGameOnly GameOnlyInputMode;
		PC->SetInputMode(GameOnlyInputMode);
		PC->bShowMouseCursor = false;

		bCanMove = true;
		bCanShoot = true;
	}
	else
	{
		FInputModeGameAndUI UIInputMode;
		UIInputMode.SetWidgetToFocus(nullptr); 
		PC->SetInputMode(UIInputMode);
		PC->bShowMouseCursor = true;

		bCanMove = false;
		bCanShoot = false;
	}
}

void AGangNamCharacter::StartBulletWidget()
{
	if (BulletWidgetClass && !BulletWidget)
	{
		BulletWidget = CreateWidget<UColorWidget>(GetWorld(), BulletWidgetClass);
		if (BulletWidget)
		{
			BulletWidget->AddToViewport(0);
		}
	}
}

void AGangNamCharacter::MakeBulletFull()
{
	if (BulletWidget)
	{
		BulletWidget->SetBackColor(0);
		CurrentBullet = MaxBullet;
	}
}




