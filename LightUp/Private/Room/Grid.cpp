// Fill out your copyright notice in the Description page of Project Settings.

#include "LightUp.h"
#include "Grid.h"

// Set the default values
UGrid::UGrid()
{
	GridCenterLocation = FVector(0, 0, 0);

	bIsValid = true;
	bIsHidden = false;
	GridIndexX = -1;
	GridIndexY = -1;
	HilbertIndex = -1;

	PreviousGrid = nullptr;
	NextGrid = nullptr;
	CrossGrid = nullptr;
	IsolateGrid = nullptr;
}

UGrid* UGrid::GetUpGrid(UGrid* ArrayHilbertGrid[8][8], uint8 Size)
{
	if ((int(GridIndexY) == Size - 1) || !ArrayHilbertGrid[int(GridIndexY) + 1][int(GridIndexX)]->bIsValid)
		return nullptr;
	else
		return ArrayHilbertGrid[GridIndexY + 1][GridIndexX];
}

UGrid* UGrid::GetDownGrid(UGrid* ArrayHilbertGrid[8][8], uint8 Size)
{
	if ((GridIndexY == 0) || !ArrayHilbertGrid[GridIndexY - 1][GridIndexX]->bIsValid)
		return nullptr;
	else
		return ArrayHilbertGrid[GridIndexY - 1][GridIndexX];
}

UGrid* UGrid::GetLeftGrid(UGrid* ArrayHilbertGrid[8][8], uint8 Size)
{
	if ((GridIndexX == 0) || !ArrayHilbertGrid[GridIndexY][GridIndexX - 1]->bIsValid)
		return nullptr;
	else
		return ArrayHilbertGrid[GridIndexY][GridIndexX - 1];
}

UGrid* UGrid::GetRightGrid(UGrid* ArrayHilbertGrid[8][8], uint8 Size)
{
	if ((GridIndexX == Size - 1) || !ArrayHilbertGrid[GridIndexY][GridIndexX + 1]->bIsValid)
		return nullptr;
	else
		return ArrayHilbertGrid[GridIndexY][GridIndexX + 1];
}
