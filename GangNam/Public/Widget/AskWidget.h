// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AskWidget.generated.h"

/**
 * 
 */
UCLASS()
class GANGNAM_API UAskWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SuspectSelect")
	int32 NowIndex = 0;

private:
	UFUNCTION(BlueprintCallable, Category = "SuspectSelect")
	void MoveUp();

	UFUNCTION(BlueprintCallable, Category = "SuspectSelect")
	void MoveDown();
	
};
