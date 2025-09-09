// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/ColorWidget.h"
#include "Components/Image.h"

void UColorWidget::SetBackColor(int8 inValue)
{
    if (!BackGroundImage) return;

    FLinearColor NewColor;

    switch (inValue)
    {
    case 0:
        NewColor = FLinearColor(0.f, 1.f, 0.f, 0.5f); // 초록
        break;
    case 1:
        NewColor = FLinearColor(0.5f, 1.f, 0.f, 0.5f); // 연두
        break;
    case 2:
        NewColor = FLinearColor(1.f, 1.f, 0.f, 0.5f); // 노랑
        break;
    case 3:
        NewColor = FLinearColor(1.f, 0.f, 0.f, 0.5f); // 빨강
        break;
    default:
        NewColor = FLinearColor(0.f, 1.f, 0.f, 0.5f); // 기본 초록
        break;
    }

    BackGroundImage->SetColorAndOpacity(NewColor);
}