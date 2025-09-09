// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ScenarioStep.generated.h"

UENUM(BlueprintType)
enum class EScenarioStepType : uint8
{
    Story,          // 스토리 화면
    Dialogue,       // 대화 진행
    Gameplay,       // 플레이어 조작 파트
    Cinematic,      // 시네마틱 재생
    NextLevel       // 다음 레벨로 전환
};

USTRUCT(BlueprintType)
struct FScenarioStep : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EScenarioStepType StepType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftClassPtr<UUserWidget> WidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "StepType == EScenarioStepType::Dialogue"))
    TSoftObjectPtr<UDataTable> DialogueDataTable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "StepType == EScenarioStepType::NextLevel"))
    FName NextLevelName;
};