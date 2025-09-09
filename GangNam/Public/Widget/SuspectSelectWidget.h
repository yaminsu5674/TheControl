// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "SuspectSelectWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSelectionTimeout);

UCLASS()
class GANGNAM_API USuspectSelectWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(meta = (BindWidget))
    UTextBlock* CountdownLabel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SuspectSelect")
    int32 NowIndex = 0;

    UPROPERTY(BlueprintAssignable)
    FOnSelectionTimeout OnSelectionTimeout;

    UFUNCTION(BlueprintCallable, Category = "Countdown")
    void StartCountdown(int32 InSeconds = 10);

private:
    int32 RemainingSeconds = 10;
    FTimerHandle CountdownHandle;

    void TickCountdown();

    UFUNCTION(BlueprintCallable, Category = "SuspectSelect")
    void MoveLeft();

    UFUNCTION(BlueprintCallable, Category = "SuspectSelect")
    void MoveRight();


};