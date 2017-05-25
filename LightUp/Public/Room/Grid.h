// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "Grid.generated.h"

/** A grid is a basic cell of a room. Used for generate the path and merge or convert to a specify room. */
UCLASS()
class LIGHTUP_API UGrid : public UObject
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties.
	UGrid();

	/**
	* Get the up grid of giving grid. Return nullptr when it is not valid.
	* @param ArrayHilbertGrid The Array to handle with.
	* @param Size Tell where the edge is.
	*/
	UGrid* GetUpGrid(UGrid* ArrayHilbertGrid[8][8], uint8 Size);

	/**
	* Get the down grid of giving grid. Return nullptr when it is no valid.
	* @param ArrayHilbertGrid The Array to handle with.
	* @param Size Tell where the edge is.
	*/
	UGrid* GetDownGrid(UGrid* ArrayHilbertGrid[8][8], uint8 Size);

	/**
	* Get the left grid of giving grid. Return nullptr when it is no valid.
	* @param ArrayHilbertGrid The Array to handle with.
	* @param Size Tell where the edge is.
	*/
	UGrid* GetLeftGrid(UGrid* ArrayHilbertGrid[8][8], uint8 Size);

	/**
	* Get the right grid of giving grid. Return nullptr when it is no valid.
	* @param ArrayHilbertGrid The Array to handle with.
	* @param Size Tell where the edge is.
	*/
	UGrid* GetRightGrid(UGrid* ArrayHilbertGrid[8][8], uint8 Size);


public:
	/** The center location of a grid, used to spawn rooms later. */
	FVector GridCenterLocation;

	/** Whether the grid is in hidden room. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Room)
	uint8 bIsValid : 1;

	/** Whether the grid is in hidden room. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Room)
	uint8 bIsHidden : 1;

	/** The X index in the original hilbert curve. */
	int8 GridIndexX;

	/** The Y index in the original hilbert curve. */
	int8 GridIndexY;

	/** The origin index of the grid in Hilbert Curve. Count from 0. */
	int8 HilbertIndex;

	/** The previous grid in room path. */
	UGrid* PreviousGrid;

	/** The next grid in room path. */
	UGrid* NextGrid;

	/** The cross grid of this grid in room path. If some grids are disabled, there may create cross road to be a alternative way to go, but cross road usually lead a dead ending path, unless the last grid of a cross road connect a previous grid which means this path become looping. */
	UGrid* CrossGrid;

	/** The isolate grid of this grid in room path. When a grid only has one direction as a choice to connect, it becomes a isolate grid of that grid. */
	UGrid* IsolateGrid;
};
