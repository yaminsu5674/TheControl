// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SoldierAnimInstance.h"
#include "Characters/SoldierCharacter.h"

void USoldierAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    if (!SoldierChar)
    {
        APawn* Pawn = TryGetPawnOwner();
        if (Pawn)
        {
            SoldierChar = Cast<ASoldierCharacter>(Pawn);
        }
    }

    if (!SoldierChar) return;

    FVector Velocity = SoldierChar->GetVelocity();
    Speed = Velocity.Size();

    FRotator ControlRot = SoldierChar->GetActorRotation();
    FVector Forward = ControlRot.Vector();
    Direction = CalculateDirection(Velocity, ControlRot);
    
    if (SoldierChar)
    {
        CurrentState = SoldierChar->CurrentState;
    }

}