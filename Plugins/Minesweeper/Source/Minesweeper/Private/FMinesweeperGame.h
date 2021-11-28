#pragma once

enum ECellType
{
	Mine,
	Normal,
};

enum EMinesweeperState
{
	GameLost,
	GameWon,
	GameInProgress
};

struct FMinesweeperCell
{
	int32 NeighbourMines;
	ECellType Type;
	bool Visible;
};


class FMinesweeperGame
{
public:
	FMinesweeperGame(int32 Width, int32 Height, int32 MineCount);

	EMinesweeperState Open(int32 X, int32 Y);

	FMinesweeperCell CellAt(int32 X, int32 Y) const;

	~FMinesweeperGame();

	int32 Width() const
	{
		return Width_;
	}
	
	int32 Height() const
	{
		return Height_;
	}
	
private:
	void InitializeGrid();
	void MakeAllVisible();
	void MarkMines();
	void OpenRecursively(int32 X, int32 Y);
	
	FMinesweeperCell** Grid;
	int32 Width_ = 0;
	int32 Height_ = 0;
	int32 MineCount_ = 0;
	int32 CellsLeft_ = 0;
};
