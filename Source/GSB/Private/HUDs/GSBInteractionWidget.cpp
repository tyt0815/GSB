// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBInteractionWidget.h"
#include "Components/VerticalBox.h"

void UGSBInteractionWidget::UpdateInteractionList(const TArray<FString>& Descriptions)
{
	InteractionList->ClearChildren();

	UWorld* World = GetWorld();
	if (World && InteractionDescriptionWidgetClass)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			for (int32 i = 0; i < Descriptions.Num(); ++i)
			{
				const FString& Description = Descriptions[i];
				UGSBInteractionDescriptionWidget* DescriptionWidget = CreateWidget<UGSBInteractionDescriptionWidget>(PlayerController, InteractionDescriptionWidgetClass);
				if (DescriptionWidget)
				{
					InteractionList->AddChild(DescriptionWidget);
					DescriptionWidget->SetDescription(FText::FromString(Description));
					UnfocusDescriptionWidget(i);
				}
			}
			
		}
	}

	UpdateFocusing(0);
}

void UGSBInteractionWidget::FocusDescriptionWidget(int32 Index)
{
	if (UGSBInteractionDescriptionWidget* DescriptionWidget = Cast<UGSBInteractionDescriptionWidget>(InteractionList->GetChildAt(Index)))
	{
		FLinearColor FocusedColorAndOpacity = DescriptionWidget->GetColorAndOpacity();
		FocusedColorAndOpacity.A = 1;
		DescriptionWidget->SetColorAndOpacity(FocusedColorAndOpacity);
	}
}

void UGSBInteractionWidget::UnfocusDescriptionWidget(int32 Index)
{
	if (UGSBInteractionDescriptionWidget* DescriptionWidget = Cast<UGSBInteractionDescriptionWidget>(InteractionList->GetChildAt(Index)))
	{
		FLinearColor UnfocusedColorAndOpacity = DescriptionWidget->GetColorAndOpacity();
		UnfocusedColorAndOpacity.A = UnfocusedOpacity;
		DescriptionWidget->SetColorAndOpacity(UnfocusedColorAndOpacity);
	}
}

void UGSBInteractionWidget::UpdateFocusing(int32 Index)
{
	UnfocusDescriptionWidget(SelectedInteractionIndex);
	SelectedInteractionIndex = Index;
	FocusDescriptionWidget(SelectedInteractionIndex);
}
