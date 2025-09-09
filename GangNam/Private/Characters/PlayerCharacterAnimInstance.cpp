// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerCharacterAnimInstance.h"
#include "Characters/GangNamCharacter.h"

void UPlayerCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    if (!Character)
    {
        APawn* Pawn = TryGetPawnOwner();
        if (Pawn)
        {
            Character = Cast<AGangNamCharacter>(Pawn);
        }
    }

    if (!Character) return;

    FVector Velocity = Character->GetVelocity();
    Speed = Velocity.Size();

    FRotator ControlRot = Character->GetActorRotation();
    FVector Forward = ControlRot.Vector();
    Direction = CalculateDirection(Velocity, ControlRot);
   
    if (Character->GetIsUpperBodyReady())
    {
        UpperBodyAnimState = EUpperBodyAnimState::Ready;
    }
    else
    {
        UpperBodyAnimState = EUpperBodyAnimState::Idle;
    }
}



