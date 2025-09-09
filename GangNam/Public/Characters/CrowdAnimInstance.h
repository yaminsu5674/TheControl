// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CrowdCharacter.h"
#include "CrowdAnimInstance.generated.h"


UCLASS()
class UCrowdAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    float Speed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Locomotion")
    float Direction;

    UPROPERTY(BlueprintReadOnly, Category = "State")
    ECrowdState CurrentState;

protected:
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
    UPROPERTY()
    APawn* OwningPawn;

    UPROPERTY()
    ACrowdCharacter* CrowdChar;
};