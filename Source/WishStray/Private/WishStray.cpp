// copyright lololol

#include "WishStray.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, WishStray, "WishStray" );

DEFINE_LOG_CATEGORY(LogBengal)

void FWishStrayModuleImpl::StartupModule()
{
	UE_LOG(LogBengal, Log, TEXT("Initialising Wish.Com Stray!"));
	FDefaultGameModuleImpl::StartupModule();
}

void FWishStrayModuleImpl::ShutdownModule()
{
	UE_LOG(LogBengal, Log, TEXT("Shutting down Wish.Com Stray!"));
	FDefaultGameModuleImpl::ShutdownModule();
}
