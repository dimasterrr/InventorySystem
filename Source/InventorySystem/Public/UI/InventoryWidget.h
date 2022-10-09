#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Base/InventoryItem.h"
#include "InventoryWidget.generated.h"

class UInventoryComponent;
class UTextBlock;
class UInventoryCellWidget;
class UUniformGridPanel;

UCLASS()
class INVENTORYSYSTEM_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	UUniformGridPanel* ItemsGridPanel;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	UInventoryCellWidget* MoneyCellWidget;

	UPROPERTY(BlueprintReadOnly)
	TArray<UInventoryCellWidget*> CellWidgets;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventoryCellWidget> CellWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	int32 RowSize = 5;

	UPROPERTY()
	UInventoryComponent* RepresentedInventory;

public:
	FOnItemDrop OnItemDrop;

protected:
	virtual void NativeConstruct() override;

	void InitCellWidget(UInventoryCellWidget* Widget);

	UInventoryCellWidget* CreateCellWidget();

	void OnItemDropEvent(UInventoryCellWidget* From, UInventoryCellWidget* To);

	UFUNCTION(BlueprintCallable)
	void SetFilterByType(const TEnumAsByte<EItemType> Type);

public:
	UFUNCTION(BlueprintCallable)
	void Init(int32 GridSizeCount);

	UFUNCTION(BlueprintCallable)
	void SetRepresentedInventory(UInventoryComponent* Component);

	UFUNCTION(BlueprintCallable)
	UInventoryComponent* GetRepresentedInventory() const;
	
	UFUNCTION(BlueprintCallable)
	bool AddItem(const FInventorySlotInfo& SlotInfo, const FInventoryItemInfo& ItemInfo, int32 SlotPosition);

	UFUNCTION(BlueprintCallable)
	bool AddCurrencySlot(const FInventorySlotInfo& SlotInfo, const FInventoryItemInfo& ItemInfo);
};
