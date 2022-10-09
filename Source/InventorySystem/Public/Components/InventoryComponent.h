#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Base/InventoryItem.h"
#include "InventoryComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORYSYSTEM_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	TMap<int32, FInventorySlotInfo> Items;

	UPROPERTY(EditAnywhere)
	UDataTable* BaseInventorySet;

public:
	void Init();
	void SetupBaseInventorySet(UDataTable* InventorySet);

	FInventorySlotInfo* GetItem(int32 SlotIndex);
	int32 GetSlotByItemKey(const FName& Key);

	virtual void UpsertItem(int32 SlotIndex, const FInventorySlotInfo& Item);
	virtual bool RemoveItem(int32 SlotIndex);

	virtual int32 GetMaxItemAmount(int32 SlotIndex, const FInventoryItemInfo& Item) const;

	const TMap<int32, FInventorySlotInfo>& GetItems() const;
	int32 GetItemsNum() const;
	void RemoveAll();

	UFUNCTION(BlueprintCallable)
	void AddItemToSlot(const int32& ToSlot, const FInventorySlotInfo& ToItem);

	UFUNCTION(BlueprintPure)
	int32 GetEmptySlotKey() const;

	UFUNCTION(BlueprintPure)
	int32 GetCurrency();
};
