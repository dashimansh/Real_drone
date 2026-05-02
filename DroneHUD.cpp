#include "DroneHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"

ADroneHUD::ADroneHUD() {}

void ADroneHUD::UpdateData(
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
    bool  bInRTLMode)
{
    Motor_FL = InMotor_FL;
    Motor_FR = InMotor_FR;
    Motor_BL = InMotor_BL;
    Motor_BR = InMotor_BR;
    Altitude = InAltitude;
    Throttle = InThrottle;
    Pitch = InPitch;
    Roll = InRoll;
    Yaw = InYaw;
    bFPVMode = bInFPVMode;
    bJoystickMode = bInJoystickMode;
    bBraking = bInBraking;
    PosX = InPosX;
    PosY = InPosY;
    PosZ = InPosZ;
    bLoiterMode = bInLoiterMode;
    bRTLMode = bInRTLMode;
}

void ADroneHUD::DrawHUD()
{
    Super::DrawHUD();
    if (!Canvas) return;

    DrawMotorPanel();
    DrawFlightData();
    DrawInputPanel();
    DrawModePanel();
}

//----------------------------------------------------
// MOTOR PANEL — Top Left
//----------------------------------------------------
void ADroneHUD::DrawMotorPanel()
{
    float PX = 20.f, PY = 20.f;
    float PW = 240.f, PH = 230.f;

    DrawRect(ColorDark, PX, PY, PW, PH);
    DrawText(TEXT("-- MOTOR THRUST --"),
        ColorGreen, PX + 35.f, PY + 8.f,
        GEngine->GetSmallFont(), 1.2f);
    DrawLine(PX + 10, PY + 28, PX + PW - 10, PY + 28,
        ColorGreen, 0.5f);

    float BW = 85.f, BH = 90.f, Gap = 15.f;
    DrawMotorBar(PX + 15.f,
        PY + 38.f, BW, BH, Motor_FL, TEXT("FL"));
    DrawMotorBar(PX + 15.f + BW + Gap,
        PY + 38.f, BW, BH, Motor_FR, TEXT("FR"));
    DrawMotorBar(PX + 15.f,
        PY + 38.f + BH + 20.f, BW, BH, Motor_BL, TEXT("BL"));
    DrawMotorBar(PX + 15.f + BW + Gap,
        PY + 38.f + BH + 20.f, BW, BH, Motor_BR, TEXT("BR"));
}

void ADroneHUD::DrawMotorBar(float X, float Y,
    float Width, float BarH,
    float Value,
    const FString& Label)
{
    DrawRect(FLinearColor(0.1f, 0.1f, 0.1f, 0.9f),
        X, Y, Width, BarH);

    FLinearColor FC = Value > 0.8f ? ColorRed
        : Value > 0.5f ? ColorAmber
        : ColorGreen;

    float FH = BarH * FMath::Clamp(Value, 0.f, 1.f);
    DrawRect(FC, X, Y + (BarH - FH), Width, FH);

    DrawLine(X, Y, X + Width, Y, ColorWhite, 0.5f);
    DrawLine(X + Width, Y, X + Width, Y + BarH, ColorWhite, 0.5f);
    DrawLine(X + Width, Y + BarH, X, Y + BarH, ColorWhite, 0.5f);
    DrawLine(X, Y + BarH, X, Y, ColorWhite, 0.5f);

    DrawText(FString::Printf(TEXT("%.0f%%"), Value * 100.f),
        ColorWhite,
        X + Width * 0.5f - 12.f, Y + BarH * 0.5f - 6.f,
        GEngine->GetSmallFont(), 1.0f);

    DrawText(Label, ColorGreen,
        X + Width * 0.5f - 8.f, Y + BarH + 4.f,
        GEngine->GetSmallFont(), 1.1f);
}

//----------------------------------------------------
// FLIGHT DATA — Top Center
//----------------------------------------------------
void ADroneHUD::DrawFlightData()
{
    if (!Canvas) return;

    float CX = Canvas->SizeX * 0.5f;
    float PW = 230.f, PH = 200.f;
    float PX = CX - PW * 0.5f, PY = 20.f;

    DrawRect(ColorDark, PX, PY, PW, PH);
    DrawText(TEXT("-- FLIGHT DATA --"),
        ColorBlue, PX + 32.f, PY + 8.f,
        GEngine->GetSmallFont(), 1.2f);
    DrawLine(PX + 10, PY + 28, PX + PW - 10, PY + 28,
        ColorBlue, 0.5f);

    DrawText(FString::Printf(
        TEXT("ALT  : %.2f m"), Altitude),
        ColorGreen, PX + 10, PY + 36,
        GEngine->GetSmallFont(), 1.1f);
    DrawText(FString::Printf(
        TEXT("PITCH: %.1f deg"), Pitch * 45.f),
        ColorWhite, PX + 10, PY + 55,
        GEngine->GetSmallFont(), 1.1f);
    DrawText(FString::Printf(
        TEXT("ROLL : %.1f deg"), Roll * 45.f),
        ColorWhite, PX + 10, PY + 74,
        GEngine->GetSmallFont(), 1.1f);

    DrawLine(PX + 10, PY + 93, PX + PW - 10, PY + 93,
        ColorBlue, 0.5f);

    DrawText(TEXT("-- POSITION --"),
        ColorAmber, PX + 60.f, PY + 100.f,
        GEngine->GetSmallFont(), 1.1f);

    DrawText(FString::Printf(
        TEXT("X : %8.1f m"), PosX / 100.f),
        ColorAxisX, PX + 10, PY + 118,
        GEngine->GetSmallFont(), 1.1f);
    DrawText(FString::Printf(
        TEXT("Y : %8.1f m"), PosY / 100.f),
        ColorAxisY, PX + 10, PY + 136,
        GEngine->GetSmallFont(), 1.1f);
    DrawText(FString::Printf(
        TEXT("Z : %8.1f m"), PosZ / 100.f),
        ColorAxisZ, PX + 10, PY + 154,
        GEngine->GetSmallFont(), 1.1f);

    DrawLine(PX + 10, PY + 172, PX + PW - 10, PY + 172,
        ColorBlue, 0.5f);

    DrawText(TEXT("TRAIL:"),
        ColorWhite, PX + 10, PY + 180,
        GEngine->GetSmallFont(), 1.0f);
    DrawRect(ColorCyan, PX + 58, PY + 179, 18, 10);
    DrawText(TEXT("LO"),
        ColorWhite, PX + 79, PY + 180,
        GEngine->GetSmallFont(), 0.9f);
    DrawRect(FLinearColor(0.f, 1.f, 0.f, 1.f),
        PX + 100, PY + 179, 18, 10);
    DrawText(TEXT("MID"),
        ColorWhite, PX + 121, PY + 180,
        GEngine->GetSmallFont(), 0.9f);
    DrawRect(ColorRed,
        PX + 148, PY + 179, 18, 10);
    DrawText(TEXT("HI"),
        ColorWhite, PX + 169, PY + 180,
        GEngine->GetSmallFont(), 0.9f);
    DrawRect(FLinearColor(1.f, 1.f, 0.f, 1.f),
        PX + 192, PY + 179, 18, 10);
    DrawText(TEXT("CHG"),
        ColorWhite, PX + 213, PY + 180,
        GEngine->GetSmallFont(), 0.9f);
}

//----------------------------------------------------
// INPUT PANEL — Left below motors
//----------------------------------------------------
void ADroneHUD::DrawInputPanel()
{
    float PX = 20.f, PY = 270.f;
    float PW = 240.f, PH = 145.f;

    DrawRect(ColorDark, PX, PY, PW, PH);
    DrawText(TEXT("-- JOYSTICK INPUT --"),
        ColorAmber, PX + 22.f, PY + 8.f,
        GEngine->GetSmallFont(), 1.2f);
    DrawLine(PX + 10, PY + 28, PX + PW - 10, PY + 28,
        ColorAmber, 0.5f);

    float BW = 125.f, SX = PX + 80.f, RH = 25.f;
    DrawInputBar(SX, PY + 36.f, BW, Throttle, TEXT("RT/W"));
    DrawInputBar(SX, PY + 36.f + RH, BW, Pitch, TEXT("UP/DN"));
    DrawInputBar(SX, PY + 36.f + RH * 2, BW, Roll, TEXT("LT/RT"));
    DrawInputBar(SX, PY + 36.f + RH * 3, BW, Yaw, TEXT("LB/RB"));
}

void ADroneHUD::DrawInputBar(float X, float Y,
    float Width, float Value,
    const FString& Label)
{
    float BH = 15.f;
    DrawText(Label, ColorAmber,
        X - 65.f, Y, GEngine->GetSmallFont(), 1.0f);
    DrawRect(FLinearColor(0.1f, 0.1f, 0.1f, 0.9f),
        X, Y, Width, BH);
    DrawLine(X + Width * 0.5f, Y,
        X + Width * 0.5f, Y + BH,
        ColorWhite, 0.5f);

    float Center = X + Width * 0.5f;
    float FW = FMath::Abs(Value) * Width * 0.5f;
    FLinearColor FC = Value >= 0.f ? ColorGreen : ColorRed;

    if (Value >= 0.f)
        DrawRect(FC, Center, Y, FW, BH);
    else
        DrawRect(FC, Center - FW, Y, FW, BH);

    DrawText(FString::Printf(TEXT("%.2f"), Value),
        ColorWhite, X + Width + 6.f, Y,
        GEngine->GetSmallFont(), 1.0f);
}

//----------------------------------------------------
// MODE PANEL — Top Right
// Shows all modes + key hints including waypoints
//----------------------------------------------------
void ADroneHUD::DrawModePanel()
{
    if (!Canvas) return;

    float PW = 220.f, PH = 305.f;
    float PX = Canvas->SizeX - PW - 20.f;
    float PY = 20.f;

    DrawRect(ColorDark, PX, PY, PW, PH);

    // Camera mode
    DrawText(bFPVMode
        ? TEXT("CAM   : FPV MODE")
        : TEXT("CAM   : 3RD PERSON"),
        bFPVMode ? ColorAmber : ColorBlue,
        PX + 15.f, PY + 12.f,
        GEngine->GetSmallFont(), 1.2f);
    DrawLine(PX + 10, PY + 32, PX + PW - 10, PY + 32,
        ColorDark, 0.5f);

    // Input mode
    DrawText(bJoystickMode
        ? TEXT("INPUT : JOYSTICK")
        : TEXT("INPUT : KEYBOARD"),
        bJoystickMode ? ColorGreen : ColorWhite,
        PX + 15.f, PY + 40.f,
        GEngine->GetSmallFont(), 1.2f);
    DrawLine(PX + 10, PY + 60, PX + PW - 10, PY + 60,
        ColorDark, 0.5f);

    // Brake
    DrawText(bBraking
        ? TEXT("BRAKE : ACTIVE")
        : TEXT("BRAKE : OFF"),
        bBraking ? ColorRed : ColorGreen,
        PX + 15.f, PY + 68.f,
        GEngine->GetSmallFont(), 1.2f);
    DrawLine(PX + 10, PY + 88, PX + PW - 10, PY + 88,
        ColorDark, 0.5f);

    // Trail
    DrawText(TEXT("TRAIL : RECORDING"),
        ColorCyan,
        PX + 15.f, PY + 96.f,
        GEngine->GetSmallFont(), 1.2f);
    DrawLine(PX + 10, PY + 116, PX + PW - 10, PY + 116,
        ColorDark, 0.5f);

    // Loiter
    DrawText(bLoiterMode
        ? TEXT("LOITER: ACTIVE")
        : TEXT("LOITER: OFF"),
        bLoiterMode ? ColorCyan : ColorGray,
        PX + 15.f, PY + 124.f,
        GEngine->GetSmallFont(), 1.2f);
    DrawLine(PX + 10, PY + 144, PX + PW - 10, PY + 144,
        ColorDark, 0.5f);

    // RTL
    DrawText(bRTLMode
        ? TEXT("RTL   : RETURNING")
        : TEXT("RTL   : OFF"),
        bRTLMode ? ColorOrange : ColorGray,
        PX + 15.f, PY + 152.f,
        GEngine->GetSmallFont(), 1.2f);
    DrawLine(PX + 10, PY + 172, PX + PW - 10, PY + 172,
        ColorDark, 0.5f);

    // Key hints section title
    DrawText(TEXT("-- KEY HINTS --"),
        ColorAmber, PX + 50.f, PY + 180.f,
        GEngine->GetSmallFont(), 1.1f);
    DrawLine(PX + 10, PY + 196, PX + PW - 10, PY + 196,
        ColorDark, 0.5f);

    // All key hints
    DrawText(TEXT("SPACE = BRAKE"),
        ColorGray, PX + 15.f, PY + 204.f,
        GEngine->GetSmallFont(), 1.0f);
    DrawText(TEXT("C     = CLEAR TRAIL"),
        ColorGray, PX + 15.f, PY + 218.f,
        GEngine->GetSmallFont(), 1.0f);
    DrawText(TEXT("A     = TOGGLE CAM"),
        ColorGray, PX + 15.f, PY + 232.f,
        GEngine->GetSmallFont(), 1.0f);
    DrawText(TEXT("L     = LOITER"),
        ColorGray, PX + 15.f, PY + 246.f,
        GEngine->GetSmallFont(), 1.0f);
    DrawText(TEXT("R     = RTL"),
        ColorGray, PX + 15.f, PY + 260.f,
        GEngine->GetSmallFont(), 1.0f);

    DrawLine(PX + 10, PY + 274, PX + PW - 10, PY + 274,
        ColorDark, 0.5f);

    // Waypoint hints
    DrawText(TEXT("F = ADD WAYPOINT"),
        ColorCyan, PX + 15.f, PY + 282.f,
        GEngine->GetSmallFont(), 1.0f);
    DrawText(TEXT("G = START/STOP NAV"),
        ColorCyan, PX + 15.f, PY + 292.f,
        GEngine->GetSmallFont(), 1.0f);
}