/**
@author		Shun Moriya

* 全画面表示メニューのベースクラス
 * ゲームの更新を一時中断してUIの入力を許可します
 */

#include "FullScreenWidgetBase.h"
#include "Helper/Log.h"
#include <Blueprint/WidgetTree.h>
#include <Kismet/GameplayStatics.h>

UFullScreenWidgetBase::UFullScreenWidgetBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UFullScreenWidgetBase::StartFullScreenMenuOperation()
{
	if (mFullScreenMenuOperationStarted == false)
	{
		mFullScreenMenuOperationStarted = true;
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		if (APlayerController* playerController = GetValid(GetOwningPlayer()))
		{
			playerController->SetShowMouseCursor(true);
			const FInputModeUIOnly inputModeParameter;
			playerController->SetInputMode(inputModeParameter);
		}
	}
}

void UFullScreenWidgetBase::EndFullScreenMenuOperation()
{
	if (mFullScreenMenuOperationStarted == true)
	{
		mFullScreenMenuOperationStarted = false;
		if (APlayerController* playerController = GetValid(GetOwningPlayer()))
		{
			playerController->SetShowMouseCursor(false);
			const FInputModeGameOnly inputModeParameter;
			playerController->SetInputMode(inputModeParameter);
		}
		UGameplayStatics::SetGamePaused(GetWorld(), false);
	}
}

void UFullScreenWidgetBase::SetEnableChildren(const bool enable, const UWidget* withoutWidget)
{
	if (IsValid(WidgetTree))
	{
		TArray<const UWidget*> withoutWidgets;
		if (IsValid(withoutWidget))
		{
			const UWidget* parent = withoutWidget;
			while (IsValid(parent))
			{
				withoutWidgets.Add(parent);
				parent = parent->GetParent();
			}
		}

		WidgetTree->ForEachWidget([this, withoutWidgets, enable](UWidget* widget)
			{
				if (withoutWidgets.Contains(widget) == false)
				{
					widget->SetIsEnabled(enable);
				}
			}
		);
	}
}

void UFullScreenWidgetBase::NativeConstruct()
{
	StartFullScreenMenuOperation();
	Super::NativeConstruct();
}

void UFullScreenWidgetBase::NativeDestruct()
{
	Super::NativeDestruct();
	EndFullScreenMenuOperation();
}
