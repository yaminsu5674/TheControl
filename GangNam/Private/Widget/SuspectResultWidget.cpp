// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/SuspectResultWidget.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "UObject/ConstructorHelpers.h"

void USuspectResultWidget::SetResultImage(int32 InValue)
{
    if (!PortraitImage) return;

    UTexture2D* NPCTexture = nullptr;

    switch (InValue)
    {
    case -1:
        PortraitImage->SetVisibility(ESlateVisibility::Hidden);
        return; 

    case 0:
        if (Picture1)
        {
            NPCTexture = Picture1;
        }
        break;

    case 1:
        if (Picture2)
        {
            NPCTexture = Picture2;
        }
        break;

    case 2:
        if (Picture3)
        {
            NPCTexture = Picture3;
        }
        break;

    default:
        PortraitImage->SetVisibility(ESlateVisibility::Hidden);
        return;
    }

    if (NPCTexture)
    {
        PortraitImage->SetBrushFromTexture(NPCTexture);
        PortraitImage->SetVisibility(ESlateVisibility::Visible);
    }
    else
    {
        PortraitImage->SetVisibility(ESlateVisibility::Hidden);
    }
}

