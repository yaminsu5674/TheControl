// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DialogueLine.generated.h"

UENUM(BlueprintType)
enum class EDialogueSpeaker : uint8
{
    NPC,
    Player
};

USTRUCT(BlueprintType)
struct FDialogueLine : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    uint8 DialogueType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText DialogueText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "Speaker == EDialogueSpeaker::NPC"))
    TSoftObjectPtr<UTexture2D> Portrait;
};