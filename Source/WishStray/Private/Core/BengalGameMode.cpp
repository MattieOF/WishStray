// copyright lololol

#include "Core/BengalGameMode.h"

#include "Core/Character/BengalController.h"
#include "Core/Character/BengalPawn.h"

ABengalGameMode::ABengalGameMode()
{
	DefaultPawnClass      = ABengalPawn::StaticClass();
	PlayerControllerClass = ABengalController::StaticClass();
}
