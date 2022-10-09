#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.h"
#include "InventoryEquipComponent.generated.h"

struct FInventorySlotInfo;
struct FInventoryItemInfo;
enum class EItemEquipSlot : uint8;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORYSYSTEM_API UInventoryEquipComponent : public UInventoryComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	TMap<int32, EItemEquipSlot> EquipSlots;
	
public:
	UInventoryEquipComponent();

	virtual int32 GetMaxItemAmount(int32 SlotIndex, const FInventoryItemInfo& Item) const override;

	virtual void UpsertItem(int32 SlotIndex, const FInventorySlotInfo& Item) override;

	virtual bool RemoveItem(int32 SlotIndex) override;
};
