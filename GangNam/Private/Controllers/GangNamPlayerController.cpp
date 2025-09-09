// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/GangNamPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h" 
#include "GameModes/GangNamGameMode.h"
#include "GameModes/GangNamStationGameMode.h"



AGangNamPlayerController::AGangNamPlayerController()
{
	WidgetManager = CreateDefaultSubobject<UWidgetManager>(TEXT("WidgetManager"));
}

void AGangNamPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (ControllerMappingContext)
			{
				Subsystem->AddMappingContext(ControllerMappingContext, 1);
			}
		}
	}
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}

void AGangNamPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (SkipDialogueAction)
		{
			EnhancedInputComp->BindAction(SkipDialogueAction, ETriggerEvent::Started, this, &AGangNamPlayerController::OnSkipDialogue);
		}
	}
}


void AGangNamPlayerController::OnSkipDialogue(const FInputActionValue& Value)
{
	if (UWorld* World = GetWorld())
	{
		if (AGangNamGameMode* GM = Cast<AGangNamGameMode>(World->GetAuthGameMode()))
		{
			AGangNamStationGameMode* StationGM = Cast<AGangNamStationGameMode>(GM);
			if (StationGM && StationGM->IsMine)
			{
				StationGM->StartAsk();
				return;
			}
			else if (StationGM && StationGM->IsAsk)
			{
				StationGM->StartBottom();
			}
			else if (StationGM && StationGM->IsBottom)
			{
				if (StationGM->GetIsBottomTypingFinished())
				{
					StationGM->EndDialogue();
				}
			}
			else if (WidgetManager && WidgetManager->IsValidLowLevel())
			{
				if (WidgetManager && WidgetManager->GetBIsTypingFinished())
				/*if (WidgetManager)*/
				{
					GM->OnNextDialogue();
				}
			}

		}
	}
}




void AGangNamPlayerController::ShowWidget(TSubclassOf<UUserWidget> WidgetClass, int32 ZOrder)
{
	if (WidgetManager)
	{
		WidgetManager->ShowWidget(WidgetClass, ZOrder);
	}
}




void AGangNamPlayerController::RemoveWidget(UUserWidget* WidgetInstance)
{
	if (WidgetManager)
	{
		WidgetManager->RemoveWidget(WidgetInstance);
	}
}