#include "Components/InventoryManagerComponent.h"
#include "Components/InventoryComponent.h"
#include "UI/InventoryWidget.h"
#include "UI/InventoryCellWidget.h"
#include "UI/InventoryMainWidget.h"
#include "Blueprint/UserWidget.h"

void UInventoryManagerComponent::SetBaseInventory(UInventoryComponent* InventoryComponent)
{
	SelfInventoryComponent = InventoryComponent;
}

void UInventoryManagerComponent::SetBaseEquipment(UInventoryComponent* InventoryComponent)
{
	SelfInventoryEquipComponent = InventoryComponent;
}

void UInventoryManagerComponent::PrepareInventoryWidget(UInventoryMainWidget* Widget)
{
	if (!SelfInventoryComponent || !SelfInventoryEquipComponent || !InventoryItemsData || !Widget) return;
	SelfInventoryWidget = Widget;

	const auto LocalInventory = SelfInventoryWidget->GetInventoryWidget();
	LocalInventory->Init(FMath::Max(SelfInventoryComponent->GetItemsNum(), MinSize));
	LocalInventory->SetRepresentedInventory(SelfInventoryComponent);
	LocalInventory->OnItemDrop.AddUObject(this, &UInventoryManagerComponent::OnItemDropEvent);

	const auto LocalEquipment = SelfInventoryWidget->GetEquipmentWidget();
	LocalEquipment->SetRepresentedInventory(SelfInventoryEquipComponent);
	LocalEquipment->OnItemDrop.AddUObject(this, &UInventoryManagerComponent::OnItemDropEvent);

	for (auto& Item : SelfInventoryComponent->GetItems())
	{
		const auto ItemData = GetItemData(Item.Value.Id);
		if (!ItemData) continue;

		LocalInventory->AddItem(Item.Value, *ItemData, Item.Key);
	}

	for (auto& Item : SelfInventoryEquipComponent->GetItems())
	{
		const auto ItemData = GetItemData(Item.Value.Id);
		if (!ItemData) continue;

		LocalEquipment->AddItem(Item.Value, *ItemData, Item.Key);
	}
}

const FInventoryItemInfo* UInventoryManagerComponent::GetItemData(const FName& Id) const
{
	return InventoryItemsData ? InventoryItemsData->FindRow<FInventoryItemInfo>(Id, "") : nullptr;
}

void UInventoryManagerComponent::AddItem(const FName& Key, const int32& Count)
{
	if (Count == 0) return;

	const auto Item = GetItemData(Key);
	if (!Item) return;

	// Attach new slot container
	FInventorySlotInfo NewSlot;
	if (const auto FindSlot = SelfInventoryComponent->GetSlotByItemKey(Key); FindSlot != INDEX_NONE)
	{
		// Save old data and remove slot 
		NewSlot = *SelfInventoryComponent->GetItem(FindSlot);
		SelfInventoryComponent->RemoveItem(FindSlot);
	}

	// Save old target slot data
	auto AvailableItemCount = NewSlot.Count + Count;
	if (AvailableItemCount < 0) return;

	// Recreate target slots
	const auto NewItemsCount = FMath::CeilToInt((AvailableItemCount + .0f) / Item->MaxStackNumber);
	for (auto i = 0; i < NewItemsCount; ++i)
	{
		NewSlot.Count = FMath::Min(Item->MaxStackNumber, AvailableItemCount);
		AvailableItemCount -= NewSlot.Count;

		const auto SlotKey = SelfInventoryComponent->GetEmptySlotKey();
		SelfInventoryComponent->AddItemToSlot(SlotKey, NewSlot);
	}
}

void UInventoryManagerComponent::OnItemDropEvent(UInventoryCellWidget* From, UInventoryCellWidget* To)
{
	if (!From || !To) return;

	const auto FromInventoryWidget = From->GetParentWidget();
	const auto ToInventoryWidget = To->GetParentWidget();
	if (!FromInventoryWidget || !ToInventoryWidget) return;

	const auto FromInventoryComponent = FromInventoryWidget->GetRepresentedInventory();
	const auto ToInventoryComponent = ToInventoryWidget->GetRepresentedInventory();
	if (!FromInventoryComponent || !ToInventoryComponent) return;

	auto FromItem = From->GetItem();
	const auto FromItemData = GetItemData(FromItem.Id);

	auto ToItem = To->GetItem();
	const auto ToItemData = GetItemData(ToItem.Id);

	if (To->IsEmpty())
	{
		ToItem = FromItem;

		//////////////

		const auto ToMaxItem = ToInventoryComponent->GetMaxItemAmount(To->GetIndexInInventory(), *FromItemData);
		if (ToMaxItem == 0) return; // Equal items (from->to)
		// if (ToItem.Count == -1) ToItem.Count = FromItem.Count;

		ToItem.Count = FMath::Min(FromItem.Count, ToMaxItem);
		FromItem.Count = ToItem.Count - FromItem.Count;

		//////////////

		From->Erase();
		if (FromItem.Count)
		{
			From->AddItem(FromItem, *FromItemData);
			FromInventoryComponent->UpsertItem(From->GetIndexInInventory(), FromItem);
		}
		else
		{
			FromInventoryComponent->RemoveItem(From->GetIndexInInventory());
		}

		To->Erase();
		To->AddItem(ToItem, *FromItemData);
		ToInventoryComponent->UpsertItem(To->GetIndexInInventory(), ToItem);
	}
	else
	{
		// const auto ToMaxCount = ToInventoryComponent->GetMaxItemAmount(To->GetIndexInInventory(), *FromItemData);
		// if (ToMaxCount == 0) return;

		if (FromItem.Id == ToItem.Id)
		{
			const auto FromItemCache = FromItem;
			const auto IsFromRemove = ToItem.Count + FromItemCache.Count <= ToItemData->MaxStackNumber;
			FromItem.Count = IsFromRemove ? 0 : FromItemCache.Count + ToItem.Count - FromItemData->MaxStackNumber;
			ToItem.Count = IsFromRemove ? ToItem.Count + FromItemCache.Count : ToItemData->MaxStackNumber;

			// Update {from} slot
			From->Erase();
			if (IsFromRemove)
			{
				FromInventoryComponent->RemoveItem(From->GetIndexInInventory());
			}
			else
			{
				From->AddItem(FromItem, *FromItemData);
				FromInventoryComponent->UpsertItem(From->GetIndexInInventory(), FromItem);
			}

			// Update {to} slot
			To->Erase();
			To->AddItem(ToItem, *ToItemData);
			ToInventoryComponent->UpsertItem(To->GetIndexInInventory(), ToItem);
		}
		else
		{
			From->Erase();
			From->AddItem(ToItem, *ToItemData);
			FromInventoryComponent->UpsertItem(From->GetIndexInInventory(), ToItem);

			To->Erase();
			To->AddItem(FromItem, *FromItemData);
			ToInventoryComponent->UpsertItem(To->GetIndexInInventory(), FromItem);
		}
	}
}
