// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ColorWidget.generated.h"

class UImage;

/**
 * 
 */
UCLASS()
class GANGNAM_API UColorWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UImage* BackGroundImage;


public:
	void SetBackColor(int8 inValue);
	
};
