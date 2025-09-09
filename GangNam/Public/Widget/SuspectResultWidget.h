// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SuspectResultWidget.generated.h"


class UImage;
/**
 * 
 */
UCLASS()
class GANGNAM_API USuspectResultWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	UImage* PortraitImage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Suspect")
	UTexture2D* Picture1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Suspect")
	UTexture2D* Picture2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Suspect")
	UTexture2D* Picture3;

public:
	void SetResultImage(int32 inValue);
};
