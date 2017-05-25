// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define LEVEL 3
#define SIZE 8

#include "Grid.h"
#include "UObject/NoExportTypes.h"
#include "PathGenerator.generated.h"

/** four direction enum, used for rotate the U shape of the Hilbert Curve. */
UENUM(BlueprintType)
enum class EDirection : uint8
{
	DE_UP = 0   UMETA(DisplayName = "UP"),
	DE_LEFT     UMETA(DisplayName = "LEFT"),
	DE_RIGHT    UMETA(DisplayName = "RIGHT"),
	DE_DOWN     UMETA(DisplayName = "DOWN")
};

/**
 * Generate path of the rooms in level by using Hilbert Curve.
 * Use seed to achive random level design. The basic path is a Hilbert Curve of 8*8 grids. Then use seed to disable some grids and rearrange the path after each grid is disabled.
 * The final path may divide into 2 parts: Main path and the hidden path.
 */
UCLASS()
class LIGHTUP_API UPathGenerator : public UObject
{
	GENERATED_BODY()
	
public:
	/** Default Constructor. */
	UPathGenerator();

	/** Constructor with given seed. */
	UPathGenerator(const uint32 & InSeed);

	/** Return original hilbert path. **/
	FORCEINLINE TArray<UGrid*> GetHilbertPath() const { return ArrayHilbertPath; }

	/** Return rearranged main path. **/
	FORCEINLINE TArray<UGrid*> GetMainPath() const { return ArrayMainPath; }

	/** Return hidden path array. Return null when there is no hiiden path. **/
	FORCEINLINE TArray<UGrid*> GetHiddenPath() const { return ArrayHiddenPath; }

	/** Integration of all path generation functions. Call other functions to get the final path. */
	void GeneratePath();

private:
	/**
	* Calculate the starting grid of Hilbert Curve base from the direction of U Shape in Hilbert order 2. And set NextDisabledStartingGrid to starting grid.
	* @param Size Use size to tell the max grid index.
	*/
	void GenerateStartingGrid(uint8 Size);

	/**
	* Main recursive function to draw Hilbert Curve.
	* @param Level The level of the Hilbert Curve. Power of 2.
	* @param Direction The direction which the U Shape is facing to.
	*/
	void GenerateHilbertCurve(uint8 Level, EDirection Direction);

	/**
	* Handle connection of each single line to generate the origin Hilbert Curve.
	* @param Direction The direction of the line goes, also used to figure out where the other grid is.
	*/
	void ConnectHilbert(EDirection Direction);

	/**
	* Decide the next disabled grid. Return the order from last starting grid.
	* @return int8: The next disabled grid order counted from the last CurrentDisabledGridOrder.
	*/
	int8 GetNextDisabledGridOrder();

	/**
	* Set the disabled grid to none, disconnect the neightbour gird, remove the grid from ArrayMainPath.
	* @return 0: Normal situation. The disabled grid is in the midddle of ArrayMainPath.
	* @return 1: The current disabled grid is the first element in ArrayMainPath.
	* @return 2: The current disabled grid is the last element in ArrayMainPath.
	* @return 3: The current disabled grid is in ArrayHiddenPath. Means that the rest grids of the ArrayHilbertPath are all in ArrayHiddenPath.
	*/
	uint8 DisableGrid();

	/** Seperate the hidden path into two hidden path when current disabled grid is in hidden path. And change the CurrentDisabledGridOrder. */
	void SeperateHiddenPath();

	/** Check whether the current disable grid has cross grid or isolate grid and rearrange the path. */
	void RearrangeDisabledGrid();

	/**
	* Add given grid to ArrayHiddenPath, and set the bIsHidden of followed grids to true.
	* @param HiddenGrid The grid to be added to ArrayHiddenPath.
	*/
	void AddHiddenPath(UGrid* HiddenGrid);
	/**
	* Inverse order of the given grid and its pre valid grids.
	* @param Grid The first grid to be inversed. Always the first grid of a cross path.
	*/
	void InversePathOrder(UGrid* FirstGrid);

	/** Rearrange and connect the main path when a grid has been disabled. */
	void RearrangeMainPath();

	/**
	* Check neighbour grid of given grid and find the available grid to connect, which should have higher index in ArrayMainPath. If rearrange main path, the distance between two grid must be lower than 1/4 of current length of ArrayHilbertPath.
	* @param CheckedGrid The grid to be checked.
	* @param bIsRearrangePath Whether dealed with path or not. If true, the distance should be a control element, otherwise(which means dealing with disabled grid), only find the available grid with higher index.
	* @return -1: When not find grid to connect.
	* @retrun >=0: The HilbertIndex of the grid to connect.
	*/
	int8 ConnectAvailableGrid(UGrid* CheckedGrid, bool bIsRearrangePath);

	/**
	* Each time after disable a grid, recalculate ArrayMainPath.
	* @param FirstGrid Use first grid of ArrayMainPath to regenerate main path.
	*/
	void RecalculateMainPath(UGrid* FirstGrid);

public:
	/** Original grid array. */
	UGrid* ArrayHilbertGrid[SIZE][SIZE];

protected:
	/** Use seed to generate random path. Can be changed by LevelGameMode. */
	uint32 Seed;

	/** Original hilbert path. */
	UPROPERTY()
	TArray<UGrid*> ArrayHilbertPath;

	/** The main path of rearranged ArrayHilbertPath.(Not include the cross path, which can be got from the CrossGridIndex) */
	UPROPERTY()
	TArray<UGrid*> ArrayMainPath;

	/** All hidden path of rearranged ArrayHilbertPath. Just contain the first grid index of each hidden path. */
	UPROPERTY()
	TArray<UGrid*> ArrayHiddenPath;

private:
	/** The orientation of the base U shape face to. */
	EDirection BaseDirection;

	/** The X value of current grid, represent column index, to tell GeneratePath function where to connect. */
	int8 CurrentGridIndexX;

	/** The Y value of current grid, represent the row index. */
	int8 CurrentGridIndexY;

	/** The current disabled grid order in ArrayHilbertPath. Count from 0. Equals to HilbertIndex. */
	int8 CurrentDisabledGridOrder;

	/** Count the total number of disabled grids. */
	uint8 DisabledGridCounter;
};
