#include "FMinesweeperGame.h"

FMinesweeperGame::FMinesweeperGame(int Width, int Height, int MineCount)
{
	this->Width_ = Width;
	this->Height_ = Height;
	this->MineCount_ = MineCount;
	
	InitializeGrid();
	MarkMines();
	CellsLeft_ = Width * Height;
}

void FMinesweeperGame::InitializeGrid()
{
	Grid = new FMinesweeperCell*[Width_];
	for(auto i = 0; i < Width_; ++i)
	{
		Grid[i] = new FMinesweeperCell[Height_];
		for(auto j = 0; j < Height_; ++j)
		{
			FMinesweeperCell Cell;
			Cell.NeighbourMines = 0;
			Cell.Type = Normal;
			Cell.Visible = false;
			Grid[i][j] = Cell;
		}
	}
}

void FMinesweeperGame::MarkMines()
{
	if (MineCount_ > CellsLeft_)
		return; // throw something

	auto Mines = 0;
	while (Mines < MineCount_)
	{
		const int32 X = FMath::RandRange(0, Width_-1);
		const int32 Y = FMath::RandRange(0, Height_-1);
		if (Grid[X][Y].Type == Mine)
			continue;
		
		Grid[X][Y].Type = Mine;
		if (X > 0)
			Grid[X-1][Y].NeighbourMines += 1;
		if (X < Width_-1)
			Grid[X+1][Y].NeighbourMines += 1;
		if (Y > 0)
			Grid[X][Y-1].NeighbourMines += 1;
		if (Y < Height_-1)
			Grid[X][Y+1].NeighbourMines += 1;

		Mines++;
	}
}

EMinesweeperState FMinesweeperGame::Open(int X, int Y)
{
	const auto Cell = Grid[X][Y];
	if (Cell.Type == Mine)
		return GameLost;

	OpenRecursively(X, Y);
	
	return CellsLeft_ == MineCount_ ? GameWon : GameInProgress;
}

void FMinesweeperGame::OpenRecursively(int32 X, int32 Y)
{
	/* This is basically a simple flood fill / dfs */
	if (X < 0 || X == Width_ || Y < 0 || Y == Height_ || Grid[X][Y].Type == Mine || Grid[X][Y].Visible)
		return;
	
	Grid[X][Y].Visible = true;
	CellsLeft_--;
	if (Grid[X][Y].NeighbourMines == 0)
	{
		OpenRecursively(X-1, Y);
		OpenRecursively(X+1, Y);
		OpenRecursively(X, Y-1);
		OpenRecursively(X, Y+1);
	}
}

inline FMinesweeperCell FMinesweeperGame::CellAt(int32 X, int32 Y) const
{
	return Grid[X][Y];
}

FMinesweeperGame::~FMinesweeperGame()
{
	delete[] Grid;
}
