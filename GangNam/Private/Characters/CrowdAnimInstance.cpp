// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CrowdAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"


void UCrowdAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    if (!CrowdChar)
    {
        APawn* Pawn = TryGetPawnOwner();
        if (Pawn)
        {
            CrowdChar = Cast<ACrowdCharacter>(Pawn);
        }
    }

    if (!CrowdChar) return;

    FVector Velocity = CrowdChar->GetVelocity();
    Speed = Velocity.Size();

    FRotator ControlRot = CrowdChar->GetActorRotation();
    FVector Forward = ControlRot.Vector();
    Direction = CalculateDirection(Velocity, ControlRot);
    
    if (CrowdChar)
    {
        CurrentState = CrowdChar->CurrentState;
    }
}
