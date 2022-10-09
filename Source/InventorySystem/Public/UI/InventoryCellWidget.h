#pragma once

#include "CoreMinimal.h"
#include "InventoryWidget.h"
#include "Blueprint/UserWidget.h"
#include "Base/InventoryItem.h"
#include "InventoryCellWidget.generated.h"

class UTextBlock;
class UImage;

UCLASS()
class INVENTORYSYSTEM_API UInventoryCellWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	UImage* ItemIconImage;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	UTextBlock* ItemCountTextBlock;

	UPROPERTY(EditDefaultsOnly)
	bool CanDrag = true;

	UPROPERTY(EditAnywhere)
	int32 IndexInInventory = INDEX_NONE;

	UPROPERTY(BlueprintReadOnly)
	bool HasItem = false;

	UPROPERTY(BlueprintReadOnly)
	FInventorySlotInfo StoredItem;

	UPROPERTY(BlueprintReadOnly)
	UInventoryWidget* ParentWidget;
	
public:
	FOnItemDrop OnItemDrop;

protected:
	virtual void NativeConstruct() override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,

	                                  UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	                          UDragDropOperation* InOperation) override;

public:
	UFUNCTION(BlueprintCallable)
	const FInventorySlotInfo& GetItem();

	UFUNCTION(BlueprintCallable)
	bool AddItem(const FInventorySlotInfo& SlotInfo, const FInventoryItemInfo& ItemInfo);

	UFUNCTION(BlueprintCallable)
	void Erase(const bool Force = false);

	UFUNCTION(BlueprintPure)
	bool IsEmpty() const;

	// Helpers
	UFUNCTION(BlueprintCallable)
	void SetInventorySlotIndex(const int32 Index);

	UFUNCTION(BlueprintPure)
	int32 GetIndexInInventory() const;

	UFUNCTION(BlueprintCallable)
	void SetSlotVisible(bool Value);

	UFUNCTION()
	void SetParentWidget(UInventoryWidget* InventoryWidget);

	UFUNCTION()
	UInventoryWidget* GetParentWidget() const;
};
