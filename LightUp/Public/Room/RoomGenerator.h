// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Room.h"
#include "UObject/NoExportTypes.h"
#include "RoomGenerator.generated.h"

/**
 * Merge grids to rooms after the path has been generated. Use path array from UPathGenerator.
 */
UCLASS()
class LIGHTUP_API URoomGenerator : public UObject
{
	GENERATED_BODY()
	
public:
	URoomGenerator();

	// Sets default values for this actor's properties.
	URoomGenerator(uint8 FloorNumber);

private:
	/** All rooms in current floor. */
	TArray<TSubclassOf<ARoom>> ArrayRoom;
};
