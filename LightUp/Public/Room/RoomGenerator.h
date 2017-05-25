// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Grid.h"
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

	/** Constructor with given seed and floor number. */
	URoomGenerator(uint32 InSeed, uint8 InFloorNumber);

	/** Return room array. **/
	FORCEINLINE TArray<TSubclassOf<ARoom>> GetRoomArray() const { return ArrayRoom; }

protected:
	uint32 Seed;

	/** Some room should be spawnec at specific floor index.To decide the specific room from room type. Range from 0-2. */
	uint8 FloorNumber;

	/** All rooms in current floor. */
	TArray<TSubclassOf<ARoom>> ArrayRoom;

};
