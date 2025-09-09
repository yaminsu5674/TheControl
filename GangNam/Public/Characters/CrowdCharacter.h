// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "Characters/MoveInterface.h"
#include "GameFramework/Character.h"
#include "Engine/DataTable.h"
#include "Characters/GangNamCharacter.h"
#include "Components/WidgetComponent.h"
#include "CrowdCharacter.generated.h"

UENUM(BlueprintType)
enum class ECrowdState : uint8
{
    Idle,   
    Confused,
    Dead
};

UCLASS()
class GANGNAM_API ACrowdCharacter : public ACharacter, public IMoveInterface
{
    GENERATED_BODY()


protected:
    virtual void BeginPlay() override;

public:
    ACrowdCharacter();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crowd")
    ECrowdState CurrentState;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crowd")
    TArray<UAnimMontage*> IdleMontages;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crowd")
    TArray<UAnimMontage*> ConfusedMontages;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation|Death")
    TArray<UAnimMontage*> DeathMontages;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crowd")
    FVector TargetLocation;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crowd")
    float RandomIntMin = 2.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crowd")
    float RandomIntMax = 4.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crowd")
    float DefaultSpeed = 400.f;

    virtual void ChaseLocation() override;

    virtual void StopChase() override;

    virtual void PlayRandomIdleMontage() override;

    void PlayRandomConfusedMontage();

    void SetState(ECrowdState NewState);


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crowd")
    bool bInteractable = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crowd")
    class UWidgetComponent* QuestionMarkWidget;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crowd")
    UDataTable* DialogueData = nullptr;


    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crowd")
    bool bIsRunning = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crowd")
    bool bIsDead = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crowd")
    class USphereComponent* InteractionSphere;

    UPROPERTY()
    UMaterialInstanceDynamic* DynMaterial;

    UFUNCTION()
    void Hit();

    void Interact();

    void BlockInteract();

    void OnDeathMontageEnded(UAnimMontage* Montage, bool bInterrupted);

private:
    bool bPlayerInRange = false;

    UPROPERTY()
    class AGangNamCharacter* CachedPlayer;

    UFUNCTION()
    void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
        class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

};