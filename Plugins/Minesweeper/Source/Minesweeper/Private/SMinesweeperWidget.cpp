// Fill out your copyright notice in the Description page of Project Settings.


#include "SMinesweeperWidget.h"

#include "SlateOptMacros.h"
#include "Widgets/Input/SSpinBox.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "EditorStyleSet.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

#define LOCTEXT_NAMESPACE "MinesweeperWidget"
/* Color map for when a cell has neighbouring mines */
const FLinearColor GNeighbourCountToColor[] = {
	FLinearColor::Green,
	FLinearColor::Yellow,
	FLinearColor::Red,
	FLinearColor::Red,
};

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
				SNew(SBorder)
				.ColorAndOpacity(FLinearColor::White)
				[
					GridBox.ToSharedRef()
				]
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

SMinesweeperWidget::~SMinesweeperWidget()
{
	/* Free the memory when the widget is deleted */
	if (Game != nullptr)
		delete Game;
	Game = nullptr;
}

/* Creates a Slate Spinbox with a title, a value and a delegate */
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
				.MinValue(1)
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

void SMinesweeperWidget::CreateNewGame()
{
	if (Game != nullptr)
		delete Game;
	Game = new FMinesweeperGame(SelectedWidth, SelectedHeight, SelectedMines);
}

/** Creates the corresponding element depending on the cell type and state */
TSharedRef<SWidget> SMinesweeperWidget::GetSlateElementFromCell(const FMinesweeperCell& Cell, int32 X, int32 Y)
{
	if (!Cell.Visible)
	{
		return SNew(SButton)
			.OnClicked(FOnClicked::CreateSP(this, &SMinesweeperWidget::OnGridClicked, X, Y));
	}
	if (Cell.Type == Mine)
	{
		return SNew(SImage)
			.Image(FEditorStyle::GetBrush("BTEditor.DebuggerOverlay.Breakpoint.Enabled"));
	}
	if (Cell.NeighbourMines == 0)
	{
		return SNew(SImage)
		.ColorAndOpacity(FSlateColor(FLinearColor::Transparent));
	}

	// The cell has neighbour mines so we create a colored text to indicate it.
	return SNew(STextBlock)
		.Justification(ETextJustify::Center)
		.Margin(FMargin(0, 8, 0, 0))
		.Font(FCoreStyle::GetDefaultFontStyle("Bold", 11))
		.ColorAndOpacity(FSlateColor(GNeighbourCountToColor[Cell.NeighbourMines-1]))
		.Text(FText::AsNumber(Cell.NeighbourMines));
}

void SMinesweeperWidget::UpdateGrid()
{
	/* Update the grid of buttons depending on the state of the cell */
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

/* Event handlers */

FReply SMinesweeperWidget::OnGenerateClicked()
{
	if (SelectedMines > SelectedWidth * SelectedHeight)
	{
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("MoreMines", "Minesweeper cannot have more mines than cells"));
	} else
	{
		CreateNewGame();
		UpdateGrid();
	}
	return FReply::Handled();
}


FReply SMinesweeperWidget::OnGridClicked(int32 X, int32 Y)
{
	const auto State = Game->Open(X, Y);
	UpdateGrid();
	if (State == GameLost || State == GameWon)
	{
		const auto DialogMsg = State == GameLost
			? LOCTEXT("Lost", "You have lost.")
			: LOCTEXT("Won", "You have won!");
		FMessageDialog::Open(EAppMsgType::Ok, DialogMsg);
	}
	
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
