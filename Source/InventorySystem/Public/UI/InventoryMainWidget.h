#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryMainWidget.generated.h"

class UInventoryWidget;
class UInventoryManagerComponent;
UCLASS()
class INVENTORYSYSTEM_API UInventoryMainWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	UInventoryManagerComponent* ManagerComponent;

	UPROPERTY(meta=(BindWidget))
	UInventoryWidget* InventoryWidget;

	UPROPERTY(meta=(BindWidget))
	UInventoryWidget* EquipmentWidget;

public:
	UFUNCTION(BlueprintPure, Category=InventoryUI)
	UInventoryWidget* GetInventoryWidget() const;

	UFUNCTION(BlueprintPure, Category=InventoryUI)
	UInventoryWidget* GetEquipmentWidget() const;
};
