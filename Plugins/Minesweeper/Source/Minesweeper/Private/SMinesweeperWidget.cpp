// Fill out your copyright notice in the Description page of Project Settings.


#include "SMinesweeperWidget.h"

#include "SlateOptMacros.h"
#include "Widgets/Input/SSpinBox.h"
#include "Widgets/Layout/SUniformGridPanel.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

#define LOCTEXT_NAMESPACE "MinesweeperWidget"
constexpr int GDefault_Width = 8;
constexpr int GDefault_Height = 8;
constexpr int GDefault_Mines = 8;


void CreateConfiguration(const TSharedRef<SHorizontalBox>& Box, const TAttribute<FText>& Text, int32 Value)
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
			]
		];
}

TSharedRef<SWidget> SMinesweeperWidget::GetHeaderWidgets()
{
	auto Box = SNew(SHorizontalBox);
	CreateConfiguration(Box, LOCTEXT("GridWidth", "Grid Width"), GDefault_Width);
	CreateConfiguration(Box, LOCTEXT("GridHeight", "Grid Height"), GDefault_Height);
	CreateConfiguration(Box, LOCTEXT("NumberOfMines", "Number of Mines"), GDefault_Mines);
	return Box;
}

void SMinesweeperWidget::Construct(const FArguments& InArgs)
{
	GridBox = SNew(SUniformGridPanel);
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
				GridBox
			]
			
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SButton)
				.Text(LOCTEXT("MinesweeperNew", "New game"))
				.OnClicked(this, &SMinesweeperWidget::OnGenerateClicked)
			]
	];
}

void SMinesweeperWidget::CreateNewGame()
{
	if (Game != nullptr)
		delete Game;
	Game = new FMinesweeperGame(4, 4);
}

void SMinesweeperWidget::UpdateGrid()
{
	GridBox->ClearChildren();
	for(auto x = 0; x < 8; ++x)
	{
		for(auto y = 0; y < 8; ++y)
		{
			GridBox->AddSlot(x, y)
			[
				SNew(SButton)
				.OnClicked(FOnClicked::CreateSP(this, &SMinesweeperWidget::OnGridClicked, x, y))
			];
		}
	}
}

FReply SMinesweeperWidget::OnGenerateClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("I just started running"));
	return FReply::Handled();
}


FReply SMinesweeperWidget::OnGridClicked(int32 X, int32 Y)
{
	//FMessageDialog::Open(EAppMsgType::Ok, FText::Format(LOCTEXT("a", "You clicked button {0}, {1}"), X, Y));
	//FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Lost", "You have lost :("));
	return FReply::Handled();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
