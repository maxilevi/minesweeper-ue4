// Fill out your copyright notice in the Description page of Project Settings.


#include "SMinesweeperWidget.h"

#include "SlateOptMacros.h"
#include "Widgets/Input/SSpinBox.h"
#include "Widgets/Layout/SUniformGridPanel.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

#define LOCTEXT_NAMESPACE "MinesweeperWidget"
const FLinearColor GNeighbourCountToColor[] = {
	FLinearColor::Green,
	FLinearColor::Yellow,
	FLinearColor::Red,
};


void CreateConfiguration(const TSharedRef<SHorizontalBox>& Box, const TAttribute<FText>& Text, int32 Value, FOnInt32ValueChanged OnValueChanged)
{
	Box->AddSlot()
		.AutoWidth()
		.Padding(4)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(STextBlock)
				.Text(Text)
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SSpinBox<int32>)
				.Value(Value)
				.OnValueChanged(OnValueChanged)
			]
		];
}

TSharedRef<SWidget> SMinesweeperWidget::GetHeaderWidgets()
{
	auto Box = SNew(SHorizontalBox);
	CreateConfiguration(Box, LOCTEXT("GridWidth", "Grid Width"), GDefault_Width, FOnInt32ValueChanged::CreateSP(this, &SMinesweeperWidget::OnWidthChanged));
	CreateConfiguration(Box, LOCTEXT("GridHeight", "Grid Height"), GDefault_Height, FOnInt32ValueChanged::CreateSP(this, &SMinesweeperWidget::OnHeightChanged));
	CreateConfiguration(Box, LOCTEXT("NumberOfMines", "Number of Mines"), GDefault_Mines, FOnInt32ValueChanged::CreateSP(this, &SMinesweeperWidget::OnMinesChanged));
	return Box;
}

void SMinesweeperWidget::Construct(const FArguments& InArgs)
{
	SAssignNew(GridBox, SUniformGridPanel);
	CreateNewGame();
	UpdateGrid();
	ChildSlot
	[
		SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(8)
			[
				GetHeaderWidgets()
			]
			
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(8)
			[
				GridBox.ToSharedRef()
			]
			
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SButton)
				.Text(LOCTEXT("MinesweeperNew", "Create new game"))
				.OnClicked(this, &SMinesweeperWidget::OnGenerateClicked)
			]
	];
}

void SMinesweeperWidget::Destruct()
{
	if (Game != nullptr)
		delete Game;
	Game = nullptr;
}

void SMinesweeperWidget::CreateNewGame()
{
	if (Game != nullptr)
		delete Game;
	Game = new FMinesweeperGame(SelectedWidth, SelectedHeight, SelectedMines);
}

TSharedRef<SWidget> SMinesweeperWidget::GetSlateElementFromCell(const FMinesweeperCell& Cell, int32 X, int32 Y)
{
	if (!Cell.Visible)
	{
		return SNew(SButton)
			.OnClicked(FOnClicked::CreateSP(this, &SMinesweeperWidget::OnGridClicked, X, Y));
	}

	if (Cell.NeighbourMines == 0)
	{
		return SNew(STextBlock);
	}
	
	return SNew(STextBlock)
		.Justification(ETextJustify::Center)
		.ColorAndOpacity(FSlateColor(GNeighbourCountToColor[Cell.NeighbourMines-1]))
		.Text(FText::AsNumber(Cell.NeighbourMines));
}

void SMinesweeperWidget::UpdateGrid()
{
	GridBox->ClearChildren();
	GridBox->SetMinDesiredSlotHeight(2);
	GridBox->SetMinDesiredSlotWidth(2);
	for(auto x = 0; x < Game->Width(); ++x)
	{
		for(auto y = 0; y < Game->Height(); ++y)
		{
			const auto Cell = Game->CellAt(x, y);
			auto CellInterface = GetSlateElementFromCell(Cell, x, y);
			GridBox->AddSlot(x, y)
			[
				CellInterface
			];
		}
	}
}

FReply SMinesweeperWidget::OnGenerateClicked()
{
	CreateNewGame();
	UpdateGrid();
	return FReply::Handled();
}


FReply SMinesweeperWidget::OnGridClicked(int32 X, int32 Y)
{
	const auto State = Game->Open(X, Y);
	if (State == GameLost || State == GameWon)
	{
		const auto DialogMsg = State == GameLost
			? LOCTEXT("Lost", "You have lost :(")
			: LOCTEXT("Won", "You have won");
		FMessageDialog::Open(EAppMsgType::Ok, DialogMsg);
	}
	
	UpdateGrid();
	return FReply::Handled();
}

void SMinesweeperWidget::OnWidthChanged(int32 NewWidth)
{
	SelectedWidth = NewWidth;
}

void SMinesweeperWidget::OnHeightChanged(int32 NewHeight)
{
	SelectedHeight = NewHeight;
}

void SMinesweeperWidget::OnMinesChanged(int32 NewMines)
{
	SelectedMines = NewMines;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
