// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

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

	FReply OnGenerateClicked();
};
