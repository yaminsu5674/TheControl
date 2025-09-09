// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerCharacterAnimInstance.generated.h"

class AGangNamCharacter;


UENUM(BlueprintType)
enum class ECharacterInputAnimState : uint8
{
    Enabled,
    Sitting,
    SittingArmed,
};

UENUM(BlueprintType)
enum class EUpperBodyAnimState : uint8
{
    Idle,
    Ready,
};

UENUM(BlueprintType)
enum class ELowerBodyAnimState : uint8
{
    Locomotion,
    Turn
};

UCLASS()
class GANGNAM_API UPlayerCharacterAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Locomotion")
    float Speed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Locomotion")
    float Direction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UpperBody")
    ECharacterInputAnimState CharacterInputAnimState;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UpperBody")
    EUpperBodyAnimState UpperBodyAnimState;

protected:
    UPROPERTY(BlueprintReadOnly, Category = "Character")
    AGangNamCharacter* Character;
};