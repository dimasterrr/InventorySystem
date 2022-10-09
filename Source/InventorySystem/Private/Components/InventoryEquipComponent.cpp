#include "Components/InventoryEquipComponent.h"
#include "Base/EquipInterface.h"

UInventoryEquipComponent::UInventoryEquipComponent()
{
	EquipSlots.Add(0, EItemEquipSlot::Artefact);
	EquipSlots.Add(1, EItemEquipSlot::Grenade);
	EquipSlots.Add(2, EItemEquipSlot::PrimaryWeapon);
	EquipSlots.Add(3, EItemEquipSlot::SpecialWeapon);
	EquipSlots.Add(4, EItemEquipSlot::QuickAction);
	EquipSlots.Add(5, EItemEquipSlot::QuickAction2);
	EquipSlots.Add(6, EItemEquipSlot::QuickAction3);
	EquipSlots.Add(7, EItemEquipSlot::QuickAction4);
}

int32 UInventoryEquipComponent::GetMaxItemAmount(int32 SlotIndex, const FInventoryItemInfo& Item) const
{
	return Item.Type != EItemType::Equipment
	       || !EquipSlots.Contains(SlotIndex)
	       || !Item.EquipSlot.Contains(EquipSlots.FindChecked(SlotIndex))
		       ? 0
		       : Item.MaxStackNumber;
}

void UInventoryEquipComponent::UpsertItem(int32 SlotIndex, const FInventorySlotInfo& Item)
{
	const auto InventoryOwner = Cast<IEquipInterface>(GetOwner());
	if (!InventoryOwner)
	{
		Super::UpsertItem(SlotIndex, Item);
		return;
	}

	const auto Slot = EquipSlots.Contains(SlotIndex) ? EquipSlots.FindChecked(SlotIndex) : EItemEquipSlot::None;

	if (const auto EquipItem = GetItem(SlotIndex))
	{
		InventoryOwner->UnEquipItem(Slot, EquipItem->Id);
	}

	Super::UpsertItem(SlotIndex, Item);
	InventoryOwner->EquipItem(Slot, Item.Id);
}

bool UInventoryEquipComponent::RemoveItem(int32 SlotIndex)
{
	const auto InventoryOwner = Cast<IEquipInterface>(GetOwner());
	if (!InventoryOwner)
	{
		return Super::RemoveItem(SlotIndex);;
	}

	const auto Slot = EquipSlots.Contains(SlotIndex) ? EquipSlots.FindChecked(SlotIndex) : EItemEquipSlot::None;

	if (const auto EquipItem = GetItem(SlotIndex))
	{
		InventoryOwner->UnEquipItem(Slot, EquipItem->Id);
	}

	return Super::RemoveItem(SlotIndex);
}
