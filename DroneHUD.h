#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DroneHUD.generated.h"

UCLASS()
class DRONE_PACK_API ADroneHUD : public AHUD
{
    GENERATED_BODY()

public:
    ADroneHUD();
    virtual void DrawHUD() override;

    void UpdateData(
        float InMotor_FL, float InMotor_FR,
        float InMotor_BL, float InMotor_BR,
        float InAltitude,
        float InThrottle, float InPitch,
        float InRoll, float InYaw,
        bool  bInFPVMode,
        bool  bInJoystickMode,
        bool  bInBraking,
        float InPosX,
        float InPosY,
        float InPosZ,
        bool  bInLoiterMode,
        bool  bInRTLMode);

private:
    float Motor_FL = 0.f;
    float Motor_FR = 0.f;
    float Motor_BL = 0.f;
    float Motor_BR = 0.f;
    float Altitude = 0.f;
    float Throttle = 0.f;
    float Pitch = 0.f;
    float Roll = 0.f;
    float Yaw = 0.f;
    float PosX = 0.f;
    float PosY = 0.f;
    float PosZ = 0.f;
    bool  bFPVMode = false;
    bool  bJoystickMode = false;
    bool  bBraking = false;
    bool  bLoiterMode = false;
    bool  bRTLMode = false;

    void DrawMotorPanel();
    void DrawFlightData();
    void DrawInputPanel();
    void DrawModePanel();

    void DrawMotorBar(float X, float Y,
        float Width, float BarH,
        float Value,
        const FString& Label);
    void DrawInputBar(float X, float Y,
        float Width, float Value,
        const FString& Label);

    FLinearColor ColorGreen = FLinearColor(0.f, 1.f, 0.5f, 1.f);
    FLinearColor ColorRed = FLinearColor(1.f, 0.2f, 0.2f, 1.f);
    FLinearColor ColorAmber = FLinearColor(1.f, 0.7f, 0.f, 1.f);
    FLinearColor ColorWhite = FLinearColor(1.f, 1.f, 1.f, 1.f);
    FLinearColor ColorBlue = FLinearColor(0.2f, 0.6f, 1.f, 1.f);
    FLinearColor ColorCyan = FLinearColor(0.f, 1.f, 1.f, 1.f);
    FLinearColor ColorOrange = FLinearColor(1.f, 0.5f, 0.f, 1.f);
    FLinearColor ColorDark = FLinearColor(0.05f, 0.05f, 0.05f, 0.85f);
    FLinearColor ColorGray = FLinearColor(0.5f, 0.5f, 0.5f, 1.f);
    FLinearColor ColorAxisX = FLinearColor(1.f, 0.3f, 0.3f, 1.f);
    FLinearColor ColorAxisY = FLinearColor(0.3f, 1.f, 0.3f, 1.f);
    FLinearColor ColorAxisZ = FLinearColor(0.3f, 0.5f, 1.f, 1.f);
};