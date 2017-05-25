// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameModeBase.h"
#include "LightUpGameMode.generated.h"

UCLASS(minimalapi)
class ALightUpGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALightUpGameMode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};



