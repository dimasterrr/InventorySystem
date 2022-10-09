#include "Components/InventoryComponent.h"

void UInventoryComponent::Init()
{
	if (Items.IsEmpty() && BaseInventorySet)
	{
		TArray<FInventorySlotInfo*> Slots;
		BaseInventorySet->GetAllRows<FInventorySlotInfo>("", Slots);

		for (auto i = 0; i < Slots.Num(); ++i)
		{
			auto RequestedSlot = *Slots[i];
			UpsertItem(i, RequestedSlot);
		}
	}
}

void UInventoryComponent::SetupBaseInventorySet(UDataTable* InventorySet)
{
	BaseInventorySet = InventorySet;
	RemoveAll();
	Init();
}

FInventorySlotInfo* UInventoryComponent::GetItem(const int32 SlotIndex)
{
	return Items.Find(SlotIndex);
}

int32 UInventoryComponent::GetSlotByItemKey(const FName& Key)
{
	// Filter by key
	auto FilteredItems = Items.FilterByPredicate([=](const TTuple<int32, FInventorySlotInfo> Item)
	{
		return Item.Value.Id == Key;
	});
	// Sort by count
	FilteredItems.ValueStableSort([=](const auto A, const auto B) { return A.Count > B.Count; });

	if (FilteredItems.Num() == 0) return INDEX_NONE;
	return FilteredItems.begin().Key();
}

void UInventoryComponent::UpsertItem(const int32 SlotIndex, const FInventorySlotInfo& Item)
{
	RemoveItem(SlotIndex);
	Items.Add(SlotIndex, Item);
}

bool UInventoryComponent::RemoveItem(const int32 SlotIndex)
{
	return Items.Remove(SlotIndex) > 0;
}

int32 UInventoryComponent::GetMaxItemAmount(int32 SlotIndex, const FInventoryItemInfo& Item) const
{
	const auto Info = Items.Find(SlotIndex);
	return Info && Info->Id == Item.Id ? 0 : Item.MaxStackNumber;
}

const TMap<int32, FInventorySlotInfo>& UInventoryComponent::GetItems() const
{
	return Items;
}

int32 UInventoryComponent::GetItemsNum() const
{
	return Items.Num();
}

void UInventoryComponent::RemoveAll()
{
	Items.Empty();
}

void UInventoryComponent::AddItemToSlot(const int32& ToSlot, const FInventorySlotInfo& ToItem)
{
	Items.Add(ToSlot, ToItem);
}

int32 UInventoryComponent::GetEmptySlotKey() const
{
	TArray<int32> ExistingIds;
	Items.GetKeys(ExistingIds);
	ExistingIds.Sort();

	for (auto Index = 0; Index < ExistingIds.Num() - 1; ++Index)
	{
		const auto Key = ExistingIds[Index];
		if (Key > 0) return Key - 1;

		const auto NextIndex = Index + 1;
		const auto NextKey = ExistingIds[NextIndex];
		if (NextKey - Key > 1) return Key + 1;
	}

	return ExistingIds.Last() + 1;
}

int32 UInventoryComponent::GetCurrency()
{
	const auto Slot = GetSlotByItemKey("Currency.Money");
	const auto SlotInfo = GetItem(Slot);
	return Slot && SlotInfo ? SlotInfo->Count : 0;
}
