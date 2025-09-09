// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/DeathMontageNotify.h"


void UDeathMontageNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (!MeshComp) return;

    UAnimInstance* AnimInst = MeshComp->GetAnimInstance();
    if (!AnimInst) return;
    AnimInst->StopAllMontages(0.f);

    MeshComp->bPauseAnims = true;
    MeshComp->bNoSkeletonUpdate = true; 
}