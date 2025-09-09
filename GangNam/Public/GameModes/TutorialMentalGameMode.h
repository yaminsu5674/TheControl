// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameModes/GangNamGameMode.h"
#include "TutorialMentalGameMode.generated.h"

/**
 * 
 */
UCLASS()
class GANGNAM_API ATutorialMentalGameMode : public AGangNamGameMode
{
	GENERATED_BODY()

	UPROPERTY()
	UUserWidget* CurrentWidget;

	UPROPERTY()
	int32 SuspectIndex = -1;
	

protected:

	virtual void HandleDialogueStep(const FScenarioStep& Step) override;

	virtual void HandleStoryStep(const FScenarioStep& Step) override;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> SuspectWidgetClass1;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> SuspectWidgetClass3;

public:
	UFUNCTION(BlueprintCallable, Category = "SuspectSelect")
	void SetSuspectIndex(int32 inValue);

};
