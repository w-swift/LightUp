// Fill out your copyright notice in the Description page of Project Settings.

#include "LightUp.h"
#include "RoomGenerator.h"

URoomGenerator::URoomGenerator()
{
	Seed = 0;
	FloorNumber = 0;
}

URoomGenerator::URoomGenerator(uint32 InSeed, uint8 InFloorNumber)
{
	Seed = InSeed;
	FloorNumber = InFloorNumber;
}


