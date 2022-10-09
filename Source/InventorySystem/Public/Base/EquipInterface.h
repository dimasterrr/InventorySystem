#pragma once

#include "CoreMinimal.h"
#include "Base/InventoryItem.h"
#include "EquipInterface.generated.h"

UINTERFACE()
class UEquipInterface : public UInterface
{
	GENERATED_BODY()
};

class INVENTORYSYSTEM_API IEquipInterface
{
	GENERATED_BODY()

public:
	virtual void EquipItem(EItemEquipSlot Slot, FName ItemID);
	virtual void UnEquipItem(EItemEquipSlot Slot, FName ItemID);
};
