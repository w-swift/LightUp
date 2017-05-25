// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "LightUp.h"
#include "LightUpGameMode.h"
#include "LightUpPlayerController.h"
#include "LightUpCharacter.h"

ALightUpGameMode::ALightUpGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ALightUpPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ALightUpGameMode::BeginPlay()
{
	Super::BeginPlay();
}