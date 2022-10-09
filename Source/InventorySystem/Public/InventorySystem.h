#pragma once

#include "CoreMinimal.h"

class FInventorySystemModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
