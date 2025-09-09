// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Animation/AnimMontage.h"
#include "Widget/ColorWidget.h"
#include "Characters/WeaponCharacter.h"
#include "GangNamCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class ACrowdCharacter;


DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);


UENUM(BlueprintType)
enum class ELowerBodyState : uint8
{
	Locomotion,
	Turn
};

UCLASS(Blueprintable)
class AGangNamCharacter : public AWeaponCharacter
{
	GENERATED_BODY()

	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ShootAction;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ReadyAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ReloadAction;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* ReloadMontage;


	UPROPERTY(EditAnywhere, Category = "State")
	bool IsPerspective = false;

	UPROPERTY(EditAnywhere, Category = "State")
	bool IsInputEnabled = false;

	UPROPERTY(EditAnywhere, Category = "State")
	bool IsUpperBodyReady = false;


	UPROPERTY(EditAnywhere, Category = "Weapon")
	int8 CurrentBullet = 0;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	int8 MaxBullet = 30;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UColorWidget> BulletWidgetClass;
	
	UPROPERTY()
	UColorWidget* BulletWidget;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void Interact(const FInputActionValue& Value);

	void ShootInput(const FInputActionValue& Value);


	void Reload(const FInputActionValue& Value);

	void Ready(const FInputActionValue& Value);

	virtual void BeginPlay() override;

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


public:
	AGangNamCharacter();

	virtual void Shoot() override;

	void ShootAuto();

	void ReadyAuto();

	
	bool bCanMove = true;
	bool bCanShoot = true;
	bool bCanInteract = false;

	UPROPERTY()
	ACrowdCharacter* CurrentInteractTarget;


	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	void SetIsInputEnabled(bool income);

	void StartBulletWidget();

	void MakeBulletFull();




	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	FORCEINLINE bool GetIsInputEnabled() const { return IsInputEnabled;  }

	FORCEINLINE bool GetIsUpperBodyReady() const { return IsUpperBodyReady;  }

};

