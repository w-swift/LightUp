// Fill out your copyright notice in the Description page of Project Settings.

#include "LightUp.h"
#include "PathGenerator.h"

UPathGenerator::UPathGenerator()
{
	Seed = UINT32_MAX;
	
	BaseDirection = EDirection::DE_UP;

	CurrentGridIndexX = -1;
	CurrentGridIndexY = -1;
	CurrentDisabledGridOrder = 0;

	DisabledGridCounter = 0;

	// Initialize the slack of each TArray variable.
	ArrayHilbertPath.Empty(SIZE * SIZE);
	ArrayMainPath.Empty(SIZE * SIZE);
	ArrayHiddenPath.Empty(SIZE * SIZE / 2);
}

// Sets seed to the give seed value.
UPathGenerator::UPathGenerator(const uint32 & InSeed)
{
	Seed = InSeed;

	BaseDirection = EDirection::DE_UP;

	CurrentGridIndexX = -1;
	CurrentGridIndexY = -1;
	CurrentDisabledGridOrder = 0;

	DisabledGridCounter = 0;

	// Initialize the slack of each TArray variable.
	ArrayHilbertPath.Empty(SIZE * SIZE);
	ArrayMainPath.Empty(SIZE * SIZE);
	ArrayHiddenPath.Empty(SIZE * SIZE / 2);
}

void UPathGenerator::GeneratePath()
{
	for (int8 i = 0; i < SIZE; i++)
		for (int8 j = 0; j < SIZE; j++)
		{
			ArrayHilbertGrid[i][j] = new UGrid();
			ArrayHilbertGrid[i][j]->GridIndexY = i;
			ArrayHilbertGrid[i][j]->GridIndexX = j;
			///////////////////////////////////////////////////////Calculate the center location of the room.
		}

	GenerateStartingGrid(SIZE);

	GenerateHilbertCurve(LEVEL, BaseDirection);

	// Generate the original array of hilbert curve. Initialize the ArrayMainPath equal to ArrayHilbertPath.
	for (int8 i = 0; i < SIZE; i++)
		for (int8 j = 0; j < SIZE; j++)
		{
			ArrayHilbertPath.Add(ArrayHilbertGrid[i][j]);
			ArrayHilbertPath[i][j].HilbertIndex = i * SIZE + j;
			ArrayMainPath.Add(ArrayHilbertGrid[i][j]);
		}

	///////////////////////////////////////////////////////////////////Log Begin
	// UE_LOG(LogTemp, Warning, TEXT"Original Hilbert Path.(NextGrid)\n")	
	// for(int32 i = Size - 1; i >= 0; i--)
	// 	UE_LOG(LogTemp, Warning, TEXT"(%s, %s)(%s, %s)(%s, %s)(%s, %s)(%s, %s)(%s, %s)(%s, %s)(%s, %s)\n", 
	// 		ArrayHilbertPath[i*Size+0]->NextGrid->GridIndex,
	// 		ArrayHilbertPath[i*Size+1]->NextGrid->GridIndex,
	// 		ArrayHilbertPath[i*Size+2]->NextGrid->GridIndex,
	// 		ArrayHilbertPath[i*Size+3]->NextGrid->GridIndex,
	// 		ArrayHilbertPath[i*Size+4]->NextGrid->GridIndex,
	// 		ArrayHilbertPath[i*Size+5]->NextGrid->GridIndex,
	// 		ArrayHilbertPath[i*Size+6]->NextGrid->GridIndex,
	// 		ArrayHilbertPath[i*Size+7]->NextGrid->GridIndex);
	///////////////////////////////////////////////////////////////////Log End.

	while ((CurrentDisabledGridOrder = GetNextDisabledGridOrder()) != -1)
	{
		DisabledGridCounter++;

		switch (DisableGrid())
		{
			// Normal situation.The disabled grid is in the middle of ArrayMainPath.
		case 0:
			if (ArrayHilbertPath[CurrentDisabledGridOrder]->PreviousGrid)
				ArrayHilbertPath[CurrentDisabledGridOrder]->PreviousGrid->NextGrid = nullptr;
			if (ArrayHilbertPath[CurrentDisabledGridOrder]->NextGrid)
				ArrayHilbertPath[CurrentDisabledGridOrder]->NextGrid->PreviousGrid = nullptr;

			RearrangeDisabledGrid();
			RearrangeMainPath();
			break;

			// Current disabled grid is the first grid of the ArrayMainPath.
		case 1:
			if (ArrayHilbertPath[CurrentDisabledGridOrder]->NextGrid)
				ArrayHilbertPath[CurrentDisabledGridOrder]->NextGrid->PreviousGrid = nullptr;

			ArrayMainPath.RemoveAt(0);
			CurrentDisabledGridOrder++;
			break;

			// Current disabled grid is the last grid of the ArrayMainPath.
		case 2:
			if (ArrayHilbertPath[CurrentDisabledGridOrder]->PreviousGrid)
				ArrayHilbertPath[CurrentDisabledGridOrder]->PreviousGrid->NextGrid = nullptr;

			ArrayMainPath.RemoveAt(ArrayMainPath.Num() - 1);
			return;

			// Current disabled grid is in ArrayHiddenPath.
		case 3:
			SeperateHiddenPath();
			break;
		}
	}
}

void UPathGenerator::GenerateStartingGrid(uint8 Size)
{
	switch (Seed % 4)
	{
	case 0:
		BaseDirection = EDirection::DE_UP;
		CurrentGridIndexY = 0;
		CurrentGridIndexX = Size - 1;
		break;
	case 1:
		BaseDirection = EDirection::DE_LEFT;
		CurrentGridIndexY = 0;
		CurrentGridIndexX = Size - 1;
		break;
	case 2:
		BaseDirection = EDirection::DE_RIGHT;
		CurrentGridIndexY = Size - 1;
		CurrentGridIndexX = 0;
		break;
	case 3:
		BaseDirection = EDirection::DE_DOWN;
		CurrentGridIndexY = Size - 1;
		CurrentGridIndexX = 0;
		break;
	}
}

void UPathGenerator::GenerateHilbertCurve(uint8 Level, EDirection Direction)
{
	if (Level == 1)
	{
		switch (Direction)
		{
		case EDirection::DE_LEFT:
			ConnectHilbert(EDirection::DE_RIGHT);
			ConnectHilbert(EDirection::DE_DOWN);
			ConnectHilbert(EDirection::DE_LEFT);
			break;
		case EDirection::DE_RIGHT:
			ConnectHilbert(EDirection::DE_LEFT);
			ConnectHilbert(EDirection::DE_UP);
			ConnectHilbert(EDirection::DE_RIGHT);
			break;
		case EDirection::DE_UP:
			ConnectHilbert(EDirection::DE_DOWN);
			ConnectHilbert(EDirection::DE_RIGHT);
			ConnectHilbert(EDirection::DE_UP);
			break;
		case EDirection::DE_DOWN:
			ConnectHilbert(EDirection::DE_UP);
			ConnectHilbert(EDirection::DE_LEFT);
			ConnectHilbert(EDirection::DE_DOWN);
			break;
		}
	}
	else
		switch (Direction)
		{
		case EDirection::DE_LEFT:
			GenerateHilbertCurve(Level - 1, EDirection::DE_UP);
			ConnectHilbert(EDirection::DE_RIGHT);
			GenerateHilbertCurve(Level - 1, EDirection::DE_LEFT);
			ConnectHilbert(EDirection::DE_DOWN);
			GenerateHilbertCurve(Level - 1, EDirection::DE_LEFT);
			ConnectHilbert(EDirection::DE_LEFT);
			GenerateHilbertCurve(Level - 1, EDirection::DE_DOWN);
			break;
		case EDirection::DE_RIGHT:
			GenerateHilbertCurve(Level - 1, EDirection::DE_DOWN);
			ConnectHilbert(EDirection::DE_LEFT);
			GenerateHilbertCurve(Level - 1, EDirection::DE_RIGHT);
			ConnectHilbert(EDirection::DE_UP);
			GenerateHilbertCurve(Level - 1, EDirection::DE_RIGHT);
			ConnectHilbert(EDirection::DE_RIGHT);
			GenerateHilbertCurve(Level - 1, EDirection::DE_UP);
			break;
		case EDirection::DE_UP:
			GenerateHilbertCurve(Level - 1, EDirection::DE_LEFT);
			ConnectHilbert(EDirection::DE_DOWN);
			GenerateHilbertCurve(Level - 1, EDirection::DE_UP);
			ConnectHilbert(EDirection::DE_RIGHT);
			GenerateHilbertCurve(Level - 1, EDirection::DE_UP);
			ConnectHilbert(EDirection::DE_UP);
			GenerateHilbertCurve(Level - 1, EDirection::DE_RIGHT);
			break;
		case EDirection::DE_DOWN:
			GenerateHilbertCurve(Level - 1, EDirection::DE_RIGHT);
			ConnectHilbert(EDirection::DE_UP);
			GenerateHilbertCurve(Level - 1, EDirection::DE_DOWN);
			ConnectHilbert(EDirection::DE_LEFT);
			GenerateHilbertCurve(Level - 1, EDirection::DE_DOWN);
			ConnectHilbert(EDirection::DE_DOWN);
			GenerateHilbertCurve(Level - 1, EDirection::DE_LEFT);
			break;
		}
}

void UPathGenerator::ConnectHilbert(EDirection Direction)
{
	switch (Direction)
	{
	case EDirection::DE_LEFT:
		ArrayHilbertGrid[CurrentGridIndexY][CurrentGridIndexX]->NextGrid = ArrayHilbertGrid[CurrentGridIndexY][CurrentGridIndexX - 1];
		ArrayHilbertGrid[CurrentGridIndexY][CurrentGridIndexX - 1]->PreviousGrid = ArrayHilbertGrid[CurrentGridIndexY][CurrentGridIndexX];
		CurrentGridIndexX -= 1;
		break;
	case EDirection::DE_RIGHT:
		ArrayHilbertGrid[CurrentGridIndexY][CurrentGridIndexX]->NextGrid = ArrayHilbertGrid[CurrentGridIndexY][CurrentGridIndexX + 1];
		ArrayHilbertGrid[CurrentGridIndexY][CurrentGridIndexX + 1]->PreviousGrid = ArrayHilbertGrid[CurrentGridIndexY][CurrentGridIndexX];
		CurrentGridIndexX += 1;
		break;
	case EDirection::DE_UP:
		ArrayHilbertGrid[CurrentGridIndexY][CurrentGridIndexX]->NextGrid = ArrayHilbertGrid[CurrentGridIndexY + 1][CurrentGridIndexX];
		ArrayHilbertGrid[CurrentGridIndexY + 1][CurrentGridIndexX]->PreviousGrid = ArrayHilbertGrid[CurrentGridIndexY][CurrentGridIndexX];
		CurrentGridIndexY += 1;
		break;
	case EDirection::DE_DOWN:
		ArrayHilbertGrid[CurrentGridIndexY][CurrentGridIndexX]->NextGrid = ArrayHilbertGrid[CurrentGridIndexY - 1][CurrentGridIndexX];
		ArrayHilbertGrid[CurrentGridIndexY - 1][CurrentGridIndexX]->PreviousGrid = ArrayHilbertGrid[CurrentGridIndexY][CurrentGridIndexX];
		CurrentGridIndexY -= 1;
		break;
	}
}

int8 UPathGenerator::GetNextDisabledGridOrder()
{
	int8 NextDisabledGridOrder = (Seed / (DisabledGridCounter + 1)) % 8;

	// Check if the next disabled grid is out of range.
	if (ArrayHilbertPath.IsValidIndex(CurrentDisabledGridOrder + NextDisabledGridOrder))
		return CurrentDisabledGridOrder + NextDisabledGridOrder;
	else
		return -1;
}

uint8 UPathGenerator::DisableGrid()
{
	// Disabled the grid.
	ArrayHilbertPath[CurrentDisabledGridOrder]->bIsValid = false;

	// Current disabled grid is the first grid of the ArrayMainPath.
	if (CurrentDisabledGridOrder == ArrayMainPath[0]->HilbertIndex)
		return 1;

	// Current disabled grid is the last grid of the ArrayMainPath.
	else if (CurrentDisabledGridOrder == ArrayMainPath[ArrayMainPath.Num() - 1]->HilbertIndex)
		return 2;

	// Current disalbed grid is in a hidden path.
	else if (ArrayHilbertPath[CurrentDisabledGridOrder]->bIsHidden)
		return 3;

	// Normal situation.
	else
		return 0;
}

void UPathGenerator::SeperateHiddenPath()
{
	if (!ArrayHilbertPath[CurrentDisabledGridOrder]->PreviousGrid)
	{
		// Current disabled grid is a single hidden grid, just delete it. This grid should be the last grid of ArrayHilbertPath.
		if (!ArrayHilbertPath[CurrentDisabledGridOrder]->NextGrid)
		{
			for (int32 i = 0; i < ArrayHiddenPath.Num(); i++)
				if (ArrayHiddenPath[i] == ArrayHilbertPath[CurrentDisabledGridOrder])
				{
					ArrayHiddenPath.RemoveAt(i);
					return;
				}
		}

		// Current disabled grid is the first grid of a hidden path.
		else
		{
			for (int32 i = 0; i < ArrayHiddenPath.Num(); i++)
				if (ArrayHiddenPath[i] == ArrayHilbertPath[CurrentDisabledGridOrder])
				{
					ArrayHiddenPath[i] = ArrayHiddenPath[i]->NextGrid;
					ArrayHiddenPath[i]->PreviousGrid = nullptr;
				}
		}
	}

	// Current disabled grid is the last grid of a hidden path.
	else if (!ArrayHilbertPath[CurrentDisabledGridOrder]->NextGrid)
		ArrayHilbertPath[CurrentDisabledGridOrder]->PreviousGrid->NextGrid = nullptr;

	// Current disabled grid is in the middle of a hidden path.
	else
	{
		ArrayHilbertPath[CurrentDisabledGridOrder]->PreviousGrid->NextGrid = nullptr;
		ArrayHilbertPath[CurrentDisabledGridOrder]->NextGrid->PreviousGrid = nullptr;
		ArrayHiddenPath.Add(ArrayHilbertPath[CurrentDisabledGridOrder]->NextGrid);
	}

	// Set CurrentDisabledGridOrder to the next grid.
	CurrentDisabledGridOrder++;
}

void UPathGenerator::RearrangeDisabledGrid()
{
	UGrid* SpecialGrid;

	// If CurrentDisabledGrid has a isolate grid, add that grid and its following grid to hidden path.
	if ((SpecialGrid = ArrayHilbertPath[CurrentDisabledGridOrder]->IsolateGrid) != nullptr)
	{
		SpecialGrid->PreviousGrid = nullptr;
		AddHiddenPath(SpecialGrid);
	}

	// If CurrentDisabledGrid has a cross grid, check its neighbour grid which is valid and located at the higher order in ArrayHilbertPath.
	if ((SpecialGrid = ArrayHilbertPath[CurrentDisabledGridOrder]->CrossGrid) != nullptr)
		ConnectAvailableGrid(SpecialGrid, false);
}

void UPathGenerator::AddHiddenPath(UGrid* HiddenGrid)
{
	UGrid* TempGrid;

	ArrayHiddenPath.Add(HiddenGrid);

	while (HiddenGrid)
	{
		// Set current grid to hidden.
		HiddenGrid->bIsHidden = true;

		// If current grid has a cross grid, set cross grid and its followed grid to hidden.
		if ((TempGrid = HiddenGrid->CrossGrid) != nullptr)
			do
			{
				TempGrid->bIsHidden = true;
			} while ((TempGrid = TempGrid->NextGrid) != nullptr);

			// If currrent grid has a isolate grid, set isolate grid and its followed grid to hidden.
			if ((TempGrid = HiddenGrid->IsolateGrid) != nullptr)
				do
				{
					TempGrid->bIsHidden = true;
				} while ((TempGrid = TempGrid->NextGrid) != nullptr);

				HiddenGrid = HiddenGrid->NextGrid;
	}
}

void UPathGenerator::InversePathOrder(UGrid* FirstGrid)
{
	UGrid* TempGrid;

	while (FirstGrid)
	{
		TempGrid = FirstGrid->PreviousGrid;
		FirstGrid->PreviousGrid = FirstGrid->NextGrid;
		FirstGrid->NextGrid = TempGrid;
		FirstGrid = FirstGrid->NextGrid;
	}
}

void UPathGenerator::RearrangeMainPath()
{
	// Previous grid of current disabled grid.
	UGrid* PreGrid = ArrayHilbertPath[CurrentDisabledGridOrder]->PreviousGrid;

	// Temporary store the next CurrentDisabledGridOrder.
	int8 TempOrder = -1;

	// Check each grid before current disabled grid
	while (PreGrid)
	{
		UGrid* TempCrossGrid = PreGrid->CrossGrid;

		// If PreGrid have no cross grid. Check PreGrid.
		if (!TempCrossGrid)
		{
			if ((TempOrder = ConnectAvailableGrid(PreGrid, true)) != -1)
				CurrentDisabledGridOrder = TempOrder;
		}

		// If PreGrid have a cross gird. Check the cross grid but do not check the PreGrid.
		else
		{
			do
			{
				if ((TempOrder = ConnectAvailableGrid(TempCrossGrid, true)) != -1)
				{
					CurrentDisabledGridOrder = TempOrder;
					break;
				}
			} while ((TempCrossGrid = TempCrossGrid->NextGrid) != nullptr);
		}

		// If no grid is available, check the previous grid of the PreGrid.
		if (TempOrder == -1)
			PreGrid = PreGrid->PreviousGrid;

		// recalculate the main path and return when find the available to connect.
		else
		{
			RecalculateMainPath(ArrayMainPath[0]);
			return;
		}
	}

	// When not find the available grid to connect. Compare the length of the previous path and the next path. decide which should be added in ArrayHiddenPath.
	int32 MainIndex;
	ArrayMainPath.Find(ArrayHilbertPath[CurrentDisabledGridOrder], MainIndex);

	// Set CurrentDisabledGridOrder to next grid in  ArrayHilbertPath.
	CurrentDisabledGridOrder++;

	if ((MainIndex + 1) > (SIZE * SIZE - CurrentDisabledGridOrder))
	{
		AddHiddenPath(ArrayHilbertPath[CurrentDisabledGridOrder]);
		RecalculateMainPath(ArrayMainPath[0]);
	}
	else
	{
		AddHiddenPath(ArrayMainPath[0]);
		RecalculateMainPath(ArrayHilbertPath[CurrentDisabledGridOrder]);
	}
}

int8 UPathGenerator::ConnectAvailableGrid(UGrid* CheckedGrid, bool bIsRearrangePath)
{
	// Stored available grid.
	UGrid* TempGrid = nullptr;
	UGrid* Grid1 = nullptr;
	UGrid* Grid2 = nullptr;

	if (bIsRearrangePath)
	{
		if (((TempGrid = CheckedGrid->GetUpGrid(ArrayHilbertGrid, SIZE)) != nullptr) && (TempGrid->HilbertIndex > CheckedGrid->HilbertIndex) && (TempGrid->HilbertIndex - CheckedGrid->HilbertIndex < SIZE * SIZE / 4))
			Grid1 = TempGrid;
		if (((TempGrid = CheckedGrid->GetDownGrid(ArrayHilbertGrid, SIZE)) != nullptr) && (TempGrid->HilbertIndex > CheckedGrid->HilbertIndex) && (TempGrid->HilbertIndex - CheckedGrid->HilbertIndex < SIZE * SIZE / 4))
		{
			if (!Grid1)
				Grid1 = TempGrid;
			else
				Grid2 = TempGrid;
		}
		if (((TempGrid = CheckedGrid->GetLeftGrid(ArrayHilbertGrid, SIZE)) != nullptr) && (TempGrid->HilbertIndex > CheckedGrid->HilbertIndex) && (TempGrid->HilbertIndex - CheckedGrid->HilbertIndex < SIZE * SIZE / 4))
		{
			if (!Grid1)
				Grid1 = TempGrid;
			else
				Grid2 = TempGrid;
		}
		if (((TempGrid = CheckedGrid->GetRightGrid(ArrayHilbertGrid, SIZE)) != nullptr) && (TempGrid->HilbertIndex > CheckedGrid->HilbertIndex) && (TempGrid->HilbertIndex - CheckedGrid->HilbertIndex < SIZE * SIZE / 4))
		{
			if (!Grid1)
				Grid1 = TempGrid;
			else
				Grid2 = TempGrid;
		}

		// If there is no NeighbourGrid available, return false.
		if (!Grid1)
			return -1;

		// If there is one NeighbourGrid available, change the CrossGrid/NextGrid to MainPath/CrossGrid, inverse the grid order before NeighbourGrid, return true.
		else if (!Grid2)
		{
			CheckedGrid->CrossGrid = CheckedGrid->NextGrid;
			CheckedGrid->NextGrid = Grid1;
			Grid1->CrossGrid = Grid1->PreviousGrid;
			InversePathOrder(Grid1->CrossGrid);
			Grid1->PreviousGrid = CheckedGrid;

			return Grid1->HilbertIndex;
		}

		// If there are two NeighbourGrid available, pick the nearest one and do as the situation above, return true.
		else
		{
			CheckedGrid->CrossGrid = CheckedGrid->NextGrid;

			if (Grid1->HilbertIndex < Grid2->HilbertIndex)
			{
				CheckedGrid->NextGrid = Grid1;
				Grid1->CrossGrid = Grid1->PreviousGrid;
				InversePathOrder(Grid1->CrossGrid);
				Grid1->PreviousGrid = CheckedGrid;

				return Grid1->HilbertIndex;
			}
			else
			{
				CheckedGrid->NextGrid = Grid2;
				Grid2->CrossGrid = Grid2->PreviousGrid;
				InversePathOrder(Grid2->CrossGrid);
				Grid2->PreviousGrid = CheckedGrid;

				return Grid2->HilbertIndex;
			}
		}
	}

	else
	{
		if (((TempGrid = CheckedGrid->GetUpGrid(ArrayHilbertGrid, SIZE)) != nullptr) && (TempGrid->HilbertIndex > CheckedGrid->HilbertIndex))
			Grid1 = TempGrid;
		if (((TempGrid = CheckedGrid->GetDownGrid(ArrayHilbertGrid, SIZE)) != nullptr) && (TempGrid->HilbertIndex > CheckedGrid->HilbertIndex))
		{
			if (!Grid1)
				Grid1 = TempGrid;
			else
				Grid2 = TempGrid;
		}
		if (((TempGrid = CheckedGrid->GetLeftGrid(ArrayHilbertGrid, SIZE)) != nullptr) && (TempGrid->HilbertIndex > CheckedGrid->HilbertIndex))
		{
			if (!Grid1)
				Grid1 = TempGrid;
			else
				Grid2 = TempGrid;
		}
		if (((TempGrid = CheckedGrid->GetRightGrid(ArrayHilbertGrid, SIZE)) != nullptr) && (TempGrid->HilbertIndex > CheckedGrid->HilbertIndex))
		{
			if (!Grid1)
				Grid1 = TempGrid;
			else
				Grid2 = TempGrid;
		}

		// If there is no NeighbourGrid available, add the cross grid to the ArrayHiddenPath.
		if (!Grid1)
		{
			CheckedGrid->PreviousGrid = nullptr;
			AddHiddenPath(CheckedGrid);

			return -1;
		}

		// If there is one NeighbourGrid available, set the cross grid as the isolate grid of the NeighbourGrid.
		else if (!Grid2)
		{
			CheckedGrid->PreviousGrid = Grid1;
			Grid1->IsolateGrid = CheckedGrid;

			return Grid1->HilbertIndex;
		}

		// If there are two NeighbourGrid available, pick the nearest one and set the cross grid as cross grid of the NeighbourGrid.
		else
		{
			if (Grid1->HilbertIndex < Grid2->HilbertIndex)
			{
				CheckedGrid->PreviousGrid = Grid1;
				Grid1->CrossGrid = CheckedGrid;

				return Grid1->HilbertIndex;
			}
			else
			{
				CheckedGrid->PreviousGrid = Grid2;
				Grid2->CrossGrid = CheckedGrid;

				return Grid2->HilbertIndex;
			}
		}
	}
}

void UPathGenerator::RecalculateMainPath(UGrid* FirstGrid)
{
	ArrayMainPath.Empty(SIZE*SIZE);

	while (FirstGrid)
	{
		ArrayMainPath.Add(FirstGrid);
		FirstGrid = FirstGrid->NextGrid;
	}
}
