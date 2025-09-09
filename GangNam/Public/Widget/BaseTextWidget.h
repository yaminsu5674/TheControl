// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/DialogueLine.h"
#include "BaseTextWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class GANGNAM_API UBaseTextWidget : public UUserWidget
{
	GENERATED_BODY()
	

protected:
    UPROPERTY(meta = (BindWidget))
    UTextBlock* BaseTextBlock;

    UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
    bool bIsTypingFinished = false;

    UPROPERTY()
    FText FullDialogueText;

    UPROPERTY()
    int32 CurrentCharIndex;

    UPROPERTY()
    FTimerHandle TypingTimerHandle;

    UPROPERTY()
    float TypingInterval = 0.1f;

    void DisplayNextCharacter();


public:
    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    virtual void SetDialogueLine(const FDialogueLine& Dialogue);

    bool GetBIsTypingFinished();
};
