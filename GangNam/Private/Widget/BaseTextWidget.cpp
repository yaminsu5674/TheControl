// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/BaseTextWidget.h"
#include "Components/TextBlock.h"
#include "TimerManager.h"


void UBaseTextWidget::SetDialogueLine(const FDialogueLine& Dialogue)
{
    if (!BaseTextBlock) return;

    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(TypingTimerHandle);
    }

    FString RawString = Dialogue.DialogueText.ToString();
    RawString.ReplaceInline(TEXT("<NewLine>"), TEXT("\n"));

    FullDialogueText = FText::FromString(RawString);
    CurrentCharIndex = 0;
    bIsTypingFinished = false;
    BaseTextBlock->SetText(FText::FromString(""));

    if (GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(
            TypingTimerHandle,
            this,
            &UBaseTextWidget::DisplayNextCharacter,
            TypingInterval,
            true
        );
    }
}

void UBaseTextWidget::DisplayNextCharacter()
{
    if (!BaseTextBlock) return;

    FString FullString = FullDialogueText.ToString();
    if (CurrentCharIndex >= FullString.Len())
    {
        if (GetWorld())
        {
            GetWorld()->GetTimerManager().ClearTimer(TypingTimerHandle);
        }

        bIsTypingFinished = true;
        return;
    }

    FString CurrentText = FullString.Left(CurrentCharIndex + 1);
    BaseTextBlock->SetText(FText::FromString(CurrentText));

    CurrentCharIndex++;
}

bool UBaseTextWidget::GetBIsTypingFinished()
{
    return bIsTypingFinished;
}