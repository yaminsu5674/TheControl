// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SoldierCharacter.h"
#include "SoldierAnimInstance.generated.h"

/**
 * 
 */



UCLASS()
class GANGNAM_API USoldierAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Locomotion")
    float Speed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Locomotion")
    float Direction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UpperBody")
    ESoldierState CurrentState;

protected:
    UPROPERTY(BlueprintReadOnly, Category = "Character")
    ASoldierCharacter* SoldierChar;
	
};
