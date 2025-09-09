// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/BaseImageTextWidget.h"
#include "Components/Image.h"



void UBaseImageTextWidget::SetDialogueLine(const FDialogueLine& Dialogue)
{
	Super::SetDialogueLine(Dialogue);

    if (Dialogue.Portrait.IsValid() || Dialogue.Portrait.ToSoftObjectPath().IsValid())
    {
        UTexture2D* NPCTexture = Dialogue.Portrait.LoadSynchronous();
        if (PortraitImage && NPCTexture)
        {
            PortraitImage->SetBrushFromTexture(NPCTexture);
            PortraitImage->SetVisibility(ESlateVisibility::Visible);
        }
    }
    else
    {
        if (PortraitImage)
        {
            PortraitImage->SetVisibility(ESlateVisibility::Collapsed); 
        }
    }
}