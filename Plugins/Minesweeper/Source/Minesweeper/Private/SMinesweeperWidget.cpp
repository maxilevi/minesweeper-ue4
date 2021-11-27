// Fill out your copyright notice in the Description page of Project Settings.


#include "SMinesweeperWidget.h"

#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

#define LOCTEXT_NAMESPACE "MinesweeperWidget"

void SMinesweeperWidget::Construct(const FArguments& InArgs)
{
	
	ChildSlot
	[
		SNew(SButton)
			.Text(LOCTEXT("MinesweeperNew", "Generate new grid"))
			.OnClicked(this, &SMinesweeperWidget::OnGenerateClicked)
	];
	
}

FReply SMinesweeperWidget::OnGenerateClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("I just started running"));
	return FReply::Handled();
}

/*
FReply SMinesweeperWidget::OnGridClicked()
{
	 FMessageDialog::Open(EAppMsgType::Ok, DialogText);
}*/

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
