// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FMinesweeperGame.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Layout/SUniformGridPanel.h"

/**
 * Widget for the Minesweeper
 */
class MINESWEEPER_API SMinesweeperWidget : public SCompoundWidget
{
	
public:
	SLATE_BEGIN_ARGS(SMinesweeperWidget)
		{
		}

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	/** Destroys de widget and deletes allocated heap memory */
	virtual ~SMinesweeperWidget() override;
private:
	const int GDefault_Width = 8;
	const int GDefault_Height = 8;
	const int GDefault_Mines = 8;
	
	TSharedRef<SWidget> GetSlateElementFromCell(const FMinesweeperCell& Cell, int32 X, int32 Y);
	void UpdateGrid();
	void CreateNewGame();
	TSharedRef<SWidget> GetHeaderWidgets();

	FReply OnGenerateClicked();
	FReply OnGridClicked(int32 X, int32 Y);
	void OnWidthChanged(int32 NewWidth);
	void OnHeightChanged(int32 NewHeight);
	void OnMinesChanged(int32 NewMines);
	
	TSharedPtr<SUniformGridPanel> GridBox;
	FMinesweeperGame* Game = nullptr;
	int32 SelectedWidth = GDefault_Width;
	int32 SelectedHeight = GDefault_Height;
	int32 SelectedMines = GDefault_Mines;
};
