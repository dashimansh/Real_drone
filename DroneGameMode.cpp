#include "DroneGameMode.h"
#include "DroneHUD.h"
#include "DronePawn.h"

ADroneGameMode::ADroneGameMode()
{
    HUDClass = ADroneHUD::StaticClass();
    DefaultPawnClass = ADronePawn::StaticClass();
}