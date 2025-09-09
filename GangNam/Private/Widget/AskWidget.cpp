// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/AskWidget.h"

void UAskWidget::MoveUp()
{
    NowIndex = (NowIndex - 1 + 4) % 4;
}

void UAskWidget::MoveDown()
{
    NowIndex = (NowIndex + 1) % 4;
}