// copyright lololol

#pragma once

#include "CoreMinimal.h"

#define ECC_WALKABLE ECC_GameTraceChannel2

DECLARE_LOG_CATEGORY_EXTERN(LogBengal, Log, All)

class FWishStrayModuleImpl : public FDefaultGameModuleImpl
{
	virtual void StartupModule()  override;
	virtual void ShutdownModule() override;
};
