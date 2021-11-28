// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FMinesweeperGame.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Layout/SUniformGridPanel.h"

/**
 * 
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
	void Destruct();

	FReply OnGenerateClicked();
	FReply OnGridClicked(int32 X, int32 Y);
private:
	TSharedRef<SWidget> GetSlateElementFromCell(const FMinesweeperCell& Cell, int32 X, int32 Y);
	void UpdateGrid();
	void CreateNewGame();
	TSharedRef<SWidget> GetHeaderWidgets();
	TSharedRef<SUniformGridPanel> GridBox;
	FMinesweeperGame* Game = nullptr;
};
