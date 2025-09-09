// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/BaseTextWidget.h"
#include "BaseImageTextWidget.generated.h"


class UImage;
/**
 * 
 */
UCLASS()
class GANGNAM_API UBaseImageTextWidget : public UBaseTextWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UImage* PortraitImage;


public:
	virtual void SetDialogueLine(const FDialogueLine& Dialogue) override;
	
};
