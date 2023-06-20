// copyright lololol

#include "Core/BengalGameMode.h"

#include "Core/Character/BengalController.h"
#include "Core/Character/BengalCharacter.h"

ABengalGameMode::ABengalGameMode()
{
	DefaultPawnClass      = ABengalCharacter::StaticClass();
	PlayerControllerClass = ABengalController::StaticClass();
}
