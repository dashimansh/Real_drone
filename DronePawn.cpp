#include "DronePawn.h"
#include "DroneHUD.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"

ADronePawn::ADronePawn()
{
    PrimaryActorTick.bCanEverTick = true;

    PhysicsBody = CreateDefaultSubobject<UStaticMeshComponent>(
        TEXT("PhysicsBody"));
    RootComponent = PhysicsBody;
    PhysicsBody->SetSimulatePhysics(false);
    PhysicsBody->SetEnableGravity(false);
    PhysicsBody->SetCollisionProfileName(TEXT("NoCollision"));
    PhysicsBody->SetVisibility(false);

    BodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(
        TEXT("BodyMesh"));
    BodyMesh->SetupAttachment(RootComponent);
    BodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    BodyMesh->SetSimulatePhysics(false);
    BodyMesh->SetRelativeScale3D(FVector(100.f, 100.f, 100.f));
    BodyMesh->SetRelativeLocation(FVector::ZeroVector);
    BodyMesh->SetRelativeRotation(FRotator::ZeroRotator);

    FL_PropellerRoot = CreateDefaultSubobject<USceneComponent>(TEXT("FL_PropellerRoot"));
    FL_PropellerRoot->SetupAttachment(RootComponent);
    FL_PropellerRoot->SetRelativeLocation(FVector(45.f, -45.f, 8.f));
    FL_PropellerPivot = CreateDefaultSubobject<USceneComponent>(TEXT("FL_PropellerPivot"));
    FL_PropellerPivot->SetupAttachment(FL_PropellerRoot);
    FL_PropellerPivot->SetRelativeLocation(FVector::ZeroVector);
    FL_PropellerPivot->SetRelativeRotation(FRotator::ZeroRotator);
    FL_PropellerMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FL_PropellerMesh"));
    FL_PropellerMesh->SetupAttachment(FL_PropellerPivot);
    FL_PropellerMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    FL_PropellerMesh->SetSimulatePhysics(false);
    FL_PropellerMesh->SetRelativeLocation(FVector::ZeroVector);
    FL_PropellerMesh->SetRelativeRotation(FRotator::ZeroRotator);
    FL_PropellerMesh->SetRelativeScale3D(FVector(100.f, 100.f, 100.f));

    FR_PropellerRoot = CreateDefaultSubobject<USceneComponent>(TEXT("FR_PropellerRoot"));
    FR_PropellerRoot->SetupAttachment(RootComponent);
    FR_PropellerRoot->SetRelativeLocation(FVector(45.f, 45.f, 8.f));
    FR_PropellerPivot = CreateDefaultSubobject<USceneComponent>(TEXT("FR_PropellerPivot"));
    FR_PropellerPivot->SetupAttachment(FR_PropellerRoot);
    FR_PropellerPivot->SetRelativeLocation(FVector::ZeroVector);
    FR_PropellerPivot->SetRelativeRotation(FRotator::ZeroRotator);
    FR_PropellerMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FR_PropellerMesh"));
    FR_PropellerMesh->SetupAttachment(FR_PropellerPivot);
    FR_PropellerMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    FR_PropellerMesh->SetSimulatePhysics(false);
    FR_PropellerMesh->SetRelativeLocation(FVector::ZeroVector);
    FR_PropellerMesh->SetRelativeRotation(FRotator::ZeroRotator);
    FR_PropellerMesh->SetRelativeScale3D(FVector(100.f, 100.f, 100.f));

    BL_PropellerRoot = CreateDefaultSubobject<USceneComponent>(TEXT("BL_PropellerRoot"));
    BL_PropellerRoot->SetupAttachment(RootComponent);
    BL_PropellerRoot->SetRelativeLocation(FVector(-45.f, -45.f, 8.f));
    BL_PropellerPivot = CreateDefaultSubobject<USceneComponent>(TEXT("BL_PropellerPivot"));
    BL_PropellerPivot->SetupAttachment(BL_PropellerRoot);
    BL_PropellerPivot->SetRelativeLocation(FVector::ZeroVector);
    BL_PropellerPivot->SetRelativeRotation(FRotator::ZeroRotator);
    BL_PropellerMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BL_PropellerMesh"));
    BL_PropellerMesh->SetupAttachment(BL_PropellerPivot);
    BL_PropellerMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    BL_PropellerMesh->SetSimulatePhysics(false);
    BL_PropellerMesh->SetRelativeLocation(FVector::ZeroVector);
    BL_PropellerMesh->SetRelativeRotation(FRotator::ZeroRotator);
    BL_PropellerMesh->SetRelativeScale3D(FVector(100.f, 100.f, 100.f));

    BR_PropellerRoot = CreateDefaultSubobject<USceneComponent>(TEXT("BR_PropellerRoot"));
    BR_PropellerRoot->SetupAttachment(RootComponent);
    BR_PropellerRoot->SetRelativeLocation(FVector(-45.f, 45.f, 8.f));
    BR_PropellerPivot = CreateDefaultSubobject<USceneComponent>(TEXT("BR_PropellerPivot"));
    BR_PropellerPivot->SetupAttachment(BR_PropellerRoot);
    BR_PropellerPivot->SetRelativeLocation(FVector::ZeroVector);
    BR_PropellerPivot->SetRelativeRotation(FRotator::ZeroRotator);
    BR_PropellerMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BR_PropellerMesh"));
    BR_PropellerMesh->SetupAttachment(BR_PropellerPivot);
    BR_PropellerMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    BR_PropellerMesh->SetSimulatePhysics(false);
    BR_PropellerMesh->SetRelativeLocation(FVector::ZeroVector);
    BR_PropellerMesh->SetRelativeRotation(FRotator::ZeroRotator);
    BR_PropellerMesh->SetRelativeScale3D(FVector(100.f, 100.f, 100.f));

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(RootComponent);
    SpringArm->TargetArmLength = 300.f;
    SpringArm->SetRelativeRotation(FRotator(-20.f, 0.f, 0.f));
    SpringArm->bDoCollisionTest = false;
    SpringArm->bInheritPitch = false;
    SpringArm->bInheritRoll = false;
    SpringArm->bInheritYaw = true;

    ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));
    ThirdPersonCamera->SetupAttachment(SpringArm);
    ThirdPersonCamera->SetActive(true);

    FPVCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPVCamera"));
    FPVCamera->SetupAttachment(RootComponent);
    FPVCamera->SetRelativeLocation(FVector(20.f, 0.f, 5.f));
    FPVCamera->SetRelativeRotation(FRotator(-10.f, 0.f, 0.f));
    FPVCamera->SetActive(false);

    DroneHUD = nullptr;
    bBraking = false;
    bLoiterMode = false;
    bRTLMode = false;
    bRTLClimbing = false;
    bRTLReturning = false;
    bRTLHovering = false;
    bWaypointMode = false;
    CurrentWaypointIndex = 0;
}

void ADronePawn::BeginPlay()
{
    Super::BeginPlay();

    FVector StartPos = GetActorLocation();
    HomePosition = StartPos;
    LoiterPosition = StartPos;
    TargetAltitude = StartPos.Z;

    PID_Alt.Reset();

    ThirdPersonCamera->SetActive(true);
    FPVCamera->SetActive(false);

    bFPVMode = false;
    bJoystickMode = false;
    bBraking = false;
    bLoiterMode = false;
    bRTLMode = false;
    bRTLClimbing = false;
    bRTLReturning = false;
    bRTLHovering = false;
    bWaypointMode = false;
    CurrentWaypointIndex = 0;

    Input_Throttle = 0.f;
    Input_Pitch = 0.f;
    Input_Roll = 0.f;
    Input_Yaw = 0.f;
    Target_Throttle = 0.f;
    Target_Pitch = 0.f;
    Target_Roll = 0.f;
    Target_Yaw = 0.f;
    DroneVelocity = FVector::ZeroVector;
    CurrentPitch = 0.f;
    CurrentRoll = 0.f;

    SmoothMotor_FL = 0.f;
    SmoothMotor_FR = 0.f;
    SmoothMotor_BL = 0.f;
    SmoothMotor_BR = 0.f;

    LastRecordedPos = StartPos;
    FlightPath.Reserve(MaxPathPoints);
    FlightPath.Add(StartPos);

    APlayerController* PC =
        Cast<APlayerController>(GetController());
    if (PC)
        DroneHUD = Cast<ADroneHUD>(PC->GetHUD());

    if (GEngine)
        GEngine->AddOnScreenDebugMessage(
            5, 3.f, FColor::Yellow,
            TEXT(">> KEYBOARD MODE ACTIVE — F=ADD WP G=START H=CLEAR <<"));
}

void ADronePawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    CurrentAltitude = GetActorLocation().Z / 100.f;

    if (bWaypointMode)
        ApplyWaypointFlight(DeltaTime);
    else if (bLoiterMode)
        ApplyLoiterFlight(DeltaTime);
    else if (bRTLMode)
        ApplyRTLFlight(DeltaTime);
    else
        ApplyManualFlight(DeltaTime);

    UpdateMotorValues();
    SpinPropellers(DeltaTime);
    UpdateHUD();

    if (bShowTrail)
    {
        RecordFlightPath();
        DrawFlightTrail();
    }

    DrawWaypoints();
}

float ADronePawn::ComputePID(FPIDState& State,
    float Kp, float Ki, float Kd,
    float Error, float DeltaTime)
{
    float P = Kp * Error;
    State.Integral += Error * DeltaTime;
    State.Integral = FMath::Clamp(State.Integral, -50.f, 50.f);
    float I = Ki * State.Integral;
    float Derivative = (DeltaTime > 0.f)
        ? (Error - State.PrevError) / DeltaTime : 0.f;
    float D = Kd * Derivative;
    State.PrevError = Error;
    return P + I + D;
}

void ADronePawn::ApplyManualFlight(float DeltaTime)
{
    FVector Pos = GetActorLocation();

    float ThrottleInput = Input_Throttle;
    if (FMath::Abs(ThrottleInput) > 0.01f)
    {
        DroneVelocity.Z += ThrottleInput *
            ThrottleSpeed * DeltaTime;
        DroneVelocity.Z = FMath::Clamp(
            DroneVelocity.Z, -ThrottleSpeed, ThrottleSpeed);
    }
    else
        DroneVelocity.Z *= 0.85f;

    float PitchInput = Input_Pitch;
    if (FMath::Abs(PitchInput) > 0.05f)
    {
        CurrentPitch = FMath::FInterpTo(
            CurrentPitch, PitchInput * MaxTiltAngle,
            DeltaTime, 8.f);
        FVector Forward = GetActorForwardVector();
        Forward.Z = 0.f;
        Forward.Normalize();
        DroneVelocity += Forward *
            PitchInput * PitchSpeed * DeltaTime;
    }
    else
    {
        CurrentPitch = FMath::FInterpTo(
            CurrentPitch, 0.f, DeltaTime, 6.f);
        DroneVelocity.X *= FMath::Clamp(
            1.f - 4.f * DeltaTime, 0.f, 1.f);
    }

    float RollInput = Input_Roll;
    if (FMath::Abs(RollInput) > 0.05f)
    {
        CurrentRoll = FMath::FInterpTo(
            CurrentRoll, RollInput * MaxTiltAngle,
            DeltaTime, 8.f);
        FVector Right = GetActorRightVector();
        Right.Z = 0.f;
        Right.Normalize();
        DroneVelocity += Right *
            RollInput * RollSpeed * DeltaTime;
    }
    else
    {
        CurrentRoll = FMath::FInterpTo(
            CurrentRoll, 0.f, DeltaTime, 6.f);
        DroneVelocity.Y *= FMath::Clamp(
            1.f - 4.f * DeltaTime, 0.f, 1.f);
    }

    FRotator NewRot = GetActorRotation();
    NewRot.Pitch = CurrentPitch;
    NewRot.Roll = CurrentRoll;
    NewRot.Yaw += Input_Yaw * YawSpeed * DeltaTime;
    SetActorRotation(NewRot);

    float MaxHoriz = 1200.f;
    FVector Horiz(DroneVelocity.X, DroneVelocity.Y, 0.f);
    if (Horiz.Size() > MaxHoriz)
    {
        Horiz = Horiz.GetSafeNormal() * MaxHoriz;
        DroneVelocity.X = Horiz.X;
        DroneVelocity.Y = Horiz.Y;
    }

    FVector NewPos = Pos + DroneVelocity * DeltaTime;
    if (NewPos.Z < 0.f)
    {
        NewPos.Z = 0.f;
        DroneVelocity.Z = 0.f;
    }
    SetActorLocation(NewPos);
}

void ADronePawn::ApplyLoiterFlight(float DeltaTime)
{
    FVector Pos = GetActorLocation();
    FVector Error = LoiterPosition - Pos;

    DroneVelocity.X = FMath::FInterpTo(
        DroneVelocity.X, Error.X * 8.f, DeltaTime, 5.f);
    DroneVelocity.Y = FMath::FInterpTo(
        DroneVelocity.Y, Error.Y * 8.f, DeltaTime, 5.f);

    float AltError = LoiterPosition.Z - Pos.Z;
    float AltOutput = ComputePID(PID_Alt,
        Alt_Kp, Alt_Ki, Alt_Kd, AltError, DeltaTime);
    DroneVelocity.Z = FMath::Clamp(AltOutput, -300.f, 300.f);

    CurrentPitch = FMath::FInterpTo(CurrentPitch, 0.f, DeltaTime, 5.f);
    CurrentRoll = FMath::FInterpTo(CurrentRoll, 0.f, DeltaTime, 5.f);
    FRotator R = GetActorRotation();
    R.Pitch = CurrentPitch;
    R.Roll = CurrentRoll;
    SetActorRotation(R);

    FVector NewPos = Pos + DroneVelocity * DeltaTime;
    if (NewPos.Z < 0.f) { NewPos.Z = 0.f; DroneVelocity.Z = 0.f; }
    SetActorLocation(NewPos);
}

void ADronePawn::ApplyRTLFlight(float DeltaTime)
{
    FVector Pos = GetActorLocation();
    float   AltNow = Pos.Z;
    const float SafeZ = 500.f;

    if (bRTLClimbing)
    {
        float AltError = SafeZ - AltNow;
        DroneVelocity.Z = FMath::FInterpTo(
            DroneVelocity.Z,
            FMath::Clamp(AltError * 0.5f, -RTLSpeed, RTLSpeed),
            DeltaTime, 3.f);
        DroneVelocity.X *= 0.9f;
        DroneVelocity.Y *= 0.9f;

        if (FMath::Abs(AltNow - SafeZ) < 50.f)
        {
            bRTLClimbing = false; bRTLReturning = true;
            DroneVelocity.Z = 0.f;
            if (GEngine) GEngine->AddOnScreenDebugMessage(
                11, 2.f, FColor::Yellow, TEXT("RTL: 5m REACHED"));
        }
    }
    else if (bRTLReturning)
    {
        FVector ToHome(HomePosition.X - Pos.X, HomePosition.Y - Pos.Y, 0.f);
        float Dist = ToHome.Size2D();
        DroneVelocity.Z = FMath::Clamp((SafeZ - AltNow) * 0.5f, -200.f, 200.f);

        if (Dist > RTLAcceptRadius)
        {
            FVector Dir = ToHome.GetSafeNormal2D();
            float Spd = FMath::Min(Dist * 0.5f, RTLSpeed);
            DroneVelocity.X = FMath::FInterpTo(DroneVelocity.X, Dir.X * Spd, DeltaTime, 3.f);
            DroneVelocity.Y = FMath::FInterpTo(DroneVelocity.Y, Dir.Y * Spd, DeltaTime, 3.f);
            float HomeYaw = FMath::RadiansToDegrees(FMath::Atan2(ToHome.Y, ToHome.X));
            FRotator R = GetActorRotation();
            R.Yaw = FMath::FInterpTo(R.Yaw, HomeYaw, DeltaTime, 3.f);
            R.Pitch = FMath::FInterpTo(R.Pitch, 0.f, DeltaTime, 3.f);
            R.Roll = FMath::FInterpTo(R.Roll, 0.f, DeltaTime, 3.f);
            SetActorRotation(R);
        }
        else
        {
            bRTLReturning = false; bRTLHovering = true;
            DroneVelocity = FVector::ZeroVector;
            LoiterPosition = FVector(HomePosition.X, HomePosition.Y, SafeZ);
            PID_Alt.Reset();
            if (GEngine) GEngine->AddOnScreenDebugMessage(
                11, 3.f, FColor::Green, TEXT("RTL: HOME — HOVERING AT 5m"));
        }
    }
    else if (bRTLHovering)
    {
        FVector Error = LoiterPosition - Pos;
        DroneVelocity.X = FMath::FInterpTo(DroneVelocity.X, Error.X * 8.f, DeltaTime, 5.f);
        DroneVelocity.Y = FMath::FInterpTo(DroneVelocity.Y, Error.Y * 8.f, DeltaTime, 5.f);
        float AltOutput = ComputePID(PID_Alt, Alt_Kp, Alt_Ki, Alt_Kd, LoiterPosition.Z - Pos.Z, DeltaTime);
        DroneVelocity.Z = FMath::Clamp(AltOutput, -300.f, 300.f);
        CurrentPitch = FMath::FInterpTo(CurrentPitch, 0.f, DeltaTime, 5.f);
        CurrentRoll = FMath::FInterpTo(CurrentRoll, 0.f, DeltaTime, 5.f);
        FRotator R = GetActorRotation(); R.Pitch = CurrentPitch; R.Roll = CurrentRoll;
        SetActorRotation(R);
    }

    FVector NewPos = Pos + DroneVelocity * DeltaTime;
    if (NewPos.Z < 0.f) { NewPos.Z = 0.f; DroneVelocity.Z = 0.f; }
    SetActorLocation(NewPos);
}

void ADronePawn::ApplyWaypointFlight(float DeltaTime)
{
    if (Waypoints.Num() == 0)
    {
        bWaypointMode = false;
        return;
    }

    FVector Pos = GetActorLocation();
    float   AltNow = Pos.Z;
    FVector Target = Waypoints[CurrentWaypointIndex];
    float   TargetZ = WaypointAltitude;

    // Altitude hold at waypoint altitude
    float AltError = TargetZ - AltNow;
    DroneVelocity.Z = FMath::Clamp(
        AltError * 0.5f, -WaypointSpeed, WaypointSpeed);

    // Horizontal navigation
    FVector ToWP(Target.X - Pos.X, Target.Y - Pos.Y, 0.f);
    float Dist = ToWP.Size2D();

    if (Dist > WaypointAcceptRadius)
    {
        FVector Dir = ToWP.GetSafeNormal2D();
        float   Spd = FMath::Min(Dist * 0.5f, WaypointSpeed);

        DroneVelocity.X = FMath::FInterpTo(
            DroneVelocity.X, Dir.X * Spd, DeltaTime, 3.f);
        DroneVelocity.Y = FMath::FInterpTo(
            DroneVelocity.Y, Dir.Y * Spd, DeltaTime, 3.f);

        float WPYaw = FMath::RadiansToDegrees(
            FMath::Atan2(ToWP.Y, ToWP.X));
        FRotator R = GetActorRotation();
        R.Yaw = FMath::FInterpTo(R.Yaw, WPYaw, DeltaTime, 3.f);
        R.Pitch = FMath::FInterpTo(R.Pitch, 0.f, DeltaTime, 3.f);
        R.Roll = FMath::FInterpTo(R.Roll, 0.f, DeltaTime, 3.f);
        SetActorRotation(R);

        if (GEngine)
            GEngine->AddOnScreenDebugMessage(
                12, 0.05f, FColor::Cyan,
                FString::Printf(TEXT(
                    ">> WP [%d/%d] : %.0f m away | ALT %.1f m <<"),
                    CurrentWaypointIndex + 1,
                    Waypoints.Num(),
                    Dist / 100.f,
                    AltNow / 100.f));
    }
    else
    {
        if (CurrentWaypointIndex < Waypoints.Num() - 1)
        {
            CurrentWaypointIndex++;
            if (GEngine)
                GEngine->AddOnScreenDebugMessage(
                    12, 2.f, FColor::Green,
                    FString::Printf(TEXT(
                        ">> WP REACHED — NEXT [%d/%d] <<"),
                        CurrentWaypointIndex + 1,
                        Waypoints.Num()));
        }
        else
        {
            // All done — loiter at last waypoint
            bWaypointMode = false;
            bLoiterMode = true;
            LoiterPosition = FVector(Target.X, Target.Y, TargetZ);
            DroneVelocity = FVector::ZeroVector;
            PID_Alt.Reset();

            if (GEngine)
                GEngine->AddOnScreenDebugMessage(
                    12, 4.f, FColor::Green,
                    TEXT(">> ALL WAYPOINTS COMPLETE — LOITERING <<"));
        }
    }

    FVector NewPos = Pos + DroneVelocity * DeltaTime;
    if (NewPos.Z < 0.f) { NewPos.Z = 0.f; DroneVelocity.Z = 0.f; }
    SetActorLocation(NewPos);
}

void ADronePawn::DrawWaypoints()
{
    if (Waypoints.Num() == 0) return;

    for (int32 i = 0; i < Waypoints.Num(); i++)
    {
        FVector WP = Waypoints[i];
        FVector WPTop = FVector(WP.X, WP.Y, WaypointAltitude);

        FColor WPColor;
        if (i < CurrentWaypointIndex)  WPColor = FColor::Green;
        else if (i == CurrentWaypointIndex) WPColor = FColor::Yellow;
        else                                WPColor = FColor::Cyan;

        // Big sphere at nav altitude
        DrawDebugSphere(GetWorld(), WPTop,
            100.f, 16, WPColor, false, 0.05f, 0, 4.f);

        // Small sphere at ground level
        DrawDebugSphere(GetWorld(),
            FVector(WP.X, WP.Y, 0.f),
            40.f, 8, WPColor, false, 0.05f, 0, 2.f);

        // Vertical pole from ground to nav altitude
        DrawDebugLine(GetWorld(),
            FVector(WP.X, WP.Y, 0.f), WPTop,
            WPColor, false, 0.05f, 0, 3.f);

        // Line to next waypoint at nav altitude
        if (i < Waypoints.Num() - 1)
        {
            FVector NextTop = FVector(
                Waypoints[i + 1].X,
                Waypoints[i + 1].Y,
                WaypointAltitude);
            DrawDebugLine(GetWorld(),
                WPTop, NextTop,
                FColor::White, false, 0.05f, 0, 3.f);
        }
    }

    // Yellow line from drone to current target
    if (bWaypointMode && CurrentWaypointIndex < Waypoints.Num())
    {
        FVector TargetTop = FVector(
            Waypoints[CurrentWaypointIndex].X,
            Waypoints[CurrentWaypointIndex].Y,
            WaypointAltitude);
        DrawDebugLine(GetWorld(),
            GetActorLocation(), TargetTop,
            FColor::Yellow, false, 0.05f, 0, 2.f);
    }
}

//----------------------------------------------------
// ADD WAYPOINT — F key
//----------------------------------------------------
void ADronePawn::OnAddWaypoint()
{
    if (bWaypointMode)
    {
        if (GEngine)
            GEngine->AddOnScreenDebugMessage(
                13, 2.f, FColor::Red,
                TEXT(">> STOP NAV FIRST (G) TO ADD WAYPOINTS <<"));
        return;
    }

    FVector WP = GetActorLocation();
    Waypoints.Add(WP);

    if (GEngine)
        GEngine->AddOnScreenDebugMessage(
            13, 3.f, FColor::Cyan,
            FString::Printf(TEXT(
                ">> WP [%d] ADDED — X:%.0f Y:%.0f ALT:%.0f m <<"),
                Waypoints.Num(),
                WP.X / 100.f,
                WP.Y / 100.f,
                WP.Z / 100.f));
}

//----------------------------------------------------
// START/STOP WAYPOINT NAV — G key
//----------------------------------------------------
void ADronePawn::OnStartWaypoint()
{
    if (Waypoints.Num() == 0)
    {
        if (GEngine)
            GEngine->AddOnScreenDebugMessage(
                13, 3.f, FColor::Red,
                TEXT(">> NO WAYPOINTS! Press F to add <<"));
        return;
    }

    bWaypointMode = !bWaypointMode;

    if (bWaypointMode)
    {
        bLoiterMode = false;
        bRTLMode = false;
        bRTLClimbing = false;
        bRTLReturning = false;
        bRTLHovering = false;
        CurrentWaypointIndex = 0;
        DroneVelocity = FVector::ZeroVector;
        Input_Throttle = 0.f;
        Input_Pitch = 0.f;
        Input_Roll = 0.f;
        Input_Yaw = 0.f;
        PID_Alt.Reset();

        if (GEngine)
            GEngine->AddOnScreenDebugMessage(
                13, 3.f, FColor::Green,
                FString::Printf(TEXT(
                    ">> WAYPOINT NAV STARTED — %d POINTS <<"),
                    Waypoints.Num()));
    }
    else
    {
        DroneVelocity = FVector::ZeroVector;
        if (GEngine)
            GEngine->AddOnScreenDebugMessage(
                13, 2.f, FColor::White,
                TEXT(">> WAYPOINT NAV STOPPED <<"));
    }
}

//----------------------------------------------------
// CLEAR WAYPOINTS — H key
//----------------------------------------------------
void ADronePawn::OnClearWaypoints()
{
    bWaypointMode = false;
    CurrentWaypointIndex = 0;
    Waypoints.Empty();
    DroneVelocity = FVector::ZeroVector;

    if (GEngine)
        GEngine->AddOnScreenDebugMessage(
            13, 2.f, FColor::Red,
            TEXT(">> ALL WAYPOINTS CLEARED <<"));
}

void ADronePawn::UpdateMotorValues()
{
    float T = FMath::Clamp(FMath::Abs(Input_Throttle), 0.08f, 1.f);
    float P = FMath::Clamp(Input_Pitch, -1.f, 1.f);
    float R = FMath::Clamp(Input_Roll, -1.f, 1.f);
    float Y = FMath::Clamp(Input_Yaw, -1.f, 1.f);

    Motor_FL = FMath::Clamp(T + P + R + Y, 0.f, 1.f);
    Motor_FR = FMath::Clamp(T + P - R - Y, 0.f, 1.f);
    Motor_BL = FMath::Clamp(T - P + R - Y, 0.f, 1.f);
    Motor_BR = FMath::Clamp(T - P - R + Y, 0.f, 1.f);
}

void ADronePawn::SpinPropellers(float DeltaTime)
{
    const float IdleSpeed = 0.08f;
    float TargetFL = FMath::Max(Motor_FL, IdleSpeed);
    float TargetFR = FMath::Max(Motor_FR, IdleSpeed);
    float TargetBL = FMath::Max(Motor_BL, IdleSpeed);
    float TargetBR = FMath::Max(Motor_BR, IdleSpeed);

    SmoothMotor_FL = FMath::FInterpTo(SmoothMotor_FL, TargetFL, DeltaTime, PropellerSmoothSpeed);
    SmoothMotor_FR = FMath::FInterpTo(SmoothMotor_FR, TargetFR, DeltaTime, PropellerSmoothSpeed);
    SmoothMotor_BL = FMath::FInterpTo(SmoothMotor_BL, TargetBL, DeltaTime, PropellerSmoothSpeed);
    SmoothMotor_BR = FMath::FInterpTo(SmoothMotor_BR, TargetBR, DeltaTime, PropellerSmoothSpeed);

    float SpinFL = (SmoothMotor_FL * MaxRotorSpeed) * DeltaTime;
    float SpinFR = (SmoothMotor_FR * MaxRotorSpeed) * DeltaTime;
    float SpinBL = (SmoothMotor_BL * MaxRotorSpeed) * DeltaTime;
    float SpinBR = (SmoothMotor_BR * MaxRotorSpeed) * DeltaTime;

    PropAngle_FL = FMath::Fmod(PropAngle_FL - SpinFL, 360.f);
    PropAngle_FR = FMath::Fmod(PropAngle_FR + SpinFR, 360.f);
    PropAngle_BL = FMath::Fmod(PropAngle_BL + SpinBL, 360.f);
    PropAngle_BR = FMath::Fmod(PropAngle_BR - SpinBR, 360.f);

    if (FL_PropellerPivot) FL_PropellerPivot->SetRelativeRotation(FRotator(0.f, PropAngle_FL, 0.f));
    if (FR_PropellerPivot) FR_PropellerPivot->SetRelativeRotation(FRotator(0.f, PropAngle_FR, 0.f));
    if (BL_PropellerPivot) BL_PropellerPivot->SetRelativeRotation(FRotator(0.f, PropAngle_BL, 0.f));
    if (BR_PropellerPivot) BR_PropellerPivot->SetRelativeRotation(FRotator(0.f, PropAngle_BR, 0.f));
}

void ADronePawn::OnLoiterToggle()
{
    if (bRTLMode || bWaypointMode) return;
    bLoiterMode = !bLoiterMode;
    if (bLoiterMode)
    {
        LoiterPosition = GetActorLocation();
        DroneVelocity = FVector::ZeroVector;
        Input_Throttle = Input_Pitch = Input_Roll = Input_Yaw = 0.f;
        Target_Throttle = Target_Pitch = Target_Roll = Target_Yaw = 0.f;
        PID_Alt.Reset();
        if (GEngine) GEngine->AddOnScreenDebugMessage(9, 3.f, FColor::Cyan, TEXT(">> LOITER ON <<"));
    }
    else
        if (GEngine) GEngine->AddOnScreenDebugMessage(9, 2.f, FColor::White, TEXT(">> LOITER OFF <<"));
}

void ADronePawn::OnRTLToggle()
{
    bRTLMode = !bRTLMode;
    if (bRTLMode)
    {
        bLoiterMode = bWaypointMode = false;
        bRTLClimbing = true; bRTLReturning = bRTLHovering = false;
        DroneVelocity = FVector::ZeroVector;
        Input_Throttle = Input_Pitch = Input_Roll = Input_Yaw = 0.f;
        Target_Throttle = Target_Pitch = Target_Roll = Target_Yaw = 0.f;
        PID_Alt.Reset();
        if (GEngine) GEngine->AddOnScreenDebugMessage(10, 3.f, FColor::Orange, TEXT(">> RTL — GOING TO 5m THEN HOME <<"));
    }
    else
    {
        bRTLClimbing = bRTLReturning = bRTLHovering = false;
        if (GEngine) GEngine->AddOnScreenDebugMessage(10, 2.f, FColor::White, TEXT(">> RTL OFF <<"));
    }
}

void ADronePawn::RecordFlightPath()
{
    FVector CP = GetActorLocation();
    if (FVector::Dist(CP, LastRecordedPos) < PathPointDistance) return;
    if (FlightPath.Num() >= MaxPathPoints) FlightPath.RemoveAt(0);
    FlightPath.Add(CP);
    LastRecordedPos = CP;
}

void ADronePawn::DrawFlightTrail()
{
    if (FlightPath.Num() < 2) return;
    for (int32 i = 1; i < FlightPath.Num(); i++)
    {
        FVector A = FlightPath[i - 1], B = FlightPath[i];
        float AN = FMath::Clamp(B.Z / 5000.f, 0.f, 1.f);
        FColor TC = AN < 0.33f ? FColor::Cyan : AN < 0.66f ? FColor::Green : FColor::Red;
        DrawDebugLine(GetWorld(), A, B, TC, false, 0.1f, 0, TrailLineThickness);
        DrawDebugSphere(GetWorld(), B, 8.f, 4, TC, false, 0.1f);
    }
    for (int32 i = 1; i < FlightPath.Num() - 1; i++)
    {
        if (FMath::Abs(FlightPath[i].Z - FlightPath[i - 1].Z) > 100.f)
        {
            DrawDebugSphere(GetWorld(), FlightPath[i], 20.f, 8, FColor::Yellow, false, 0.1f);
            FVector GP = FlightPath[i]; GP.Z = 0.f;
            DrawDebugLine(GetWorld(), FlightPath[i], GP, FColor::Yellow, false, 0.1f, 0, 1.f);
        }
    }
}

void ADronePawn::ClearFlightPath()
{
    FlightPath.Empty();
    LastRecordedPos = GetActorLocation();
    FlushPersistentDebugLines(GetWorld());
}

void ADronePawn::UpdateHUD()
{
    if (!DroneHUD) return;
    FVector Pos = GetActorLocation();
    DroneHUD->UpdateData(
        Motor_FL, Motor_FR, Motor_BL, Motor_BR,
        CurrentAltitude,
        Input_Throttle, Input_Pitch, Input_Roll, Input_Yaw,
        bFPVMode, bJoystickMode, bBraking,
        Pos.X, Pos.Y, Pos.Z,
        bLoiterMode, bRTLMode);
}

void ADronePawn::UpdateCameras()
{
    ThirdPersonCamera->SetActive(!bFPVMode);
    FPVCamera->SetActive(bFPVMode);
}

void ADronePawn::SetupPlayerInputComponent(
    UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("ThrottleUp", this, &ADronePawn::OnThrottleUp);
    PlayerInputComponent->BindAxis("ThrottleDown", this, &ADronePawn::OnThrottleDown);
    PlayerInputComponent->BindAxis("Pitch", this, &ADronePawn::OnPitch);
    PlayerInputComponent->BindAxis("Roll", this, &ADronePawn::OnRoll);

    PlayerInputComponent->BindAction("YawLeft", IE_Pressed, this, &ADronePawn::OnYawLeft);
    PlayerInputComponent->BindAction("YawLeft", IE_Released, this, &ADronePawn::OnYawLeftReleased);
    PlayerInputComponent->BindAction("YawRight", IE_Pressed, this, &ADronePawn::OnYawRight);
    PlayerInputComponent->BindAction("YawRight", IE_Released, this, &ADronePawn::OnYawRightReleased);

    PlayerInputComponent->BindAxis("KeyThrottleUp", this, &ADronePawn::OnKeyThrottleUp);
    PlayerInputComponent->BindAxis("KeyThrottleDown", this, &ADronePawn::OnKeyThrottleDown);
    PlayerInputComponent->BindAxis("KeyPitch", this, &ADronePawn::OnKeyPitch);
    PlayerInputComponent->BindAxis("KeyRoll", this, &ADronePawn::OnKeyRoll);

    PlayerInputComponent->BindAction("KeyYawLeft", IE_Pressed, this, &ADronePawn::OnKeyYawLeft);
    PlayerInputComponent->BindAction("KeyYawLeft", IE_Released, this, &ADronePawn::OnKeyYawLeftReleased);
    PlayerInputComponent->BindAction("KeyYawRight", IE_Pressed, this, &ADronePawn::OnKeyYawRight);
    PlayerInputComponent->BindAction("KeyYawRight", IE_Released, this, &ADronePawn::OnKeyYawRightReleased);

    PlayerInputComponent->BindAction("ToggleCamera", IE_Pressed, this, &ADronePawn::OnToggleCamera);
    PlayerInputComponent->BindAction("ToggleInputMode", IE_Pressed, this, &ADronePawn::OnToggleInputMode);
    PlayerInputComponent->BindAction("Brake", IE_Pressed, this, &ADronePawn::OnBrake);
    PlayerInputComponent->BindAction("ClearTrail", IE_Pressed, this, &ADronePawn::OnClearTrail);
    PlayerInputComponent->BindAction("Loiter", IE_Pressed, this, &ADronePawn::OnLoiterToggle);
    PlayerInputComponent->BindAction("RTL", IE_Pressed, this, &ADronePawn::OnRTLToggle);

    //----------------------------------------------------
    // WAYPOINT BINDINGS
    //----------------------------------------------------
    PlayerInputComponent->BindAction("AddWaypoint",
        IE_Pressed, this, &ADronePawn::OnAddWaypoint);
    PlayerInputComponent->BindAction("StartWaypoint",
        IE_Pressed, this, &ADronePawn::OnStartWaypoint);
    PlayerInputComponent->BindAction("ClearWaypoints",
        IE_Pressed, this, &ADronePawn::OnClearWaypoints);
}

void ADronePawn::OnThrottleUp(float Value)
{
    if (!bJoystickMode || bLoiterMode || bRTLMode || bWaypointMode) return;
    Input_Throttle = FMath::Clamp(Input_Throttle + Value * GetWorld()->GetDeltaSeconds() * 2.f, -1.f, 1.f);
}
void ADronePawn::OnThrottleDown(float Value)
{
    if (!bJoystickMode || bLoiterMode || bRTLMode || bWaypointMode) return;
    Input_Throttle = FMath::Clamp(Input_Throttle - Value * GetWorld()->GetDeltaSeconds() * 2.f, -1.f, 1.f);
}
void ADronePawn::OnPitch(float Value)
{
    if (!bJoystickMode || bLoiterMode || bRTLMode || bWaypointMode) return;
    Input_Pitch = FMath::Clamp(Value, -1.f, 1.f);
}
void ADronePawn::OnRoll(float Value)
{
    if (!bJoystickMode || bLoiterMode || bRTLMode || bWaypointMode) return;
    Input_Roll = FMath::Clamp(Value, -1.f, 1.f);
}
void ADronePawn::OnYawLeft()
{
    if (!bJoystickMode || bLoiterMode || bRTLMode || bWaypointMode) return;
    Input_Yaw = -1.f;
}
void ADronePawn::OnYawLeftReleased()
{
    if (!bJoystickMode) return;
    if (Input_Yaw < 0.f) Input_Yaw = 0.f;
}
void ADronePawn::OnYawRight()
{
    if (!bJoystickMode || bLoiterMode || bRTLMode || bWaypointMode) return;
    Input_Yaw = 1.f;
}
void ADronePawn::OnYawRightReleased()
{
    if (!bJoystickMode) return;
    if (Input_Yaw > 0.f) Input_Yaw = 0.f;
}
void ADronePawn::OnKeyThrottleUp(float Value)
{
    if (bJoystickMode || bLoiterMode || bRTLMode || bWaypointMode) return;
    if (Value > 0.f)
        Input_Throttle = FMath::Clamp(Input_Throttle + Value * GetWorld()->GetDeltaSeconds() * 2.f, -1.f, 1.f);
    else
        Input_Throttle = FMath::FInterpTo(Input_Throttle, 0.f, GetWorld()->GetDeltaSeconds(), 5.f);
}
void ADronePawn::OnKeyThrottleDown(float Value)
{
    if (bJoystickMode || bLoiterMode || bRTLMode || bWaypointMode) return;
    if (Value > 0.f)
        Input_Throttle = FMath::Clamp(Input_Throttle - Value * GetWorld()->GetDeltaSeconds() * 2.f, -1.f, 1.f);
    else
        Input_Throttle = FMath::FInterpTo(Input_Throttle, 0.f, GetWorld()->GetDeltaSeconds(), 5.f);
}
void ADronePawn::OnKeyPitch(float Value)
{
    if (bJoystickMode || bLoiterMode || bRTLMode || bWaypointMode) return;
    Input_Pitch = FMath::Abs(Value) > 0.1f ? FMath::Clamp(Value, -1.f, 1.f) : 0.f;
}
void ADronePawn::OnKeyRoll(float Value)
{
    if (bJoystickMode || bLoiterMode || bRTLMode || bWaypointMode) return;
    Input_Roll = FMath::Abs(Value) > 0.1f ? FMath::Clamp(Value, -1.f, 1.f) : 0.f;
}
void ADronePawn::OnKeyYawLeft()
{
    if (bJoystickMode || bLoiterMode || bRTLMode || bWaypointMode) return;
    Input_Yaw = -1.f;
}
void ADronePawn::OnKeyYawLeftReleased()
{
    if (bJoystickMode) return;
    if (Input_Yaw < 0.f) Input_Yaw = 0.f;
}
void ADronePawn::OnKeyYawRight()
{
    if (bJoystickMode || bLoiterMode || bRTLMode || bWaypointMode) return;
    Input_Yaw = 1.f;
}
void ADronePawn::OnKeyYawRightReleased()
{
    if (bJoystickMode) return;
    if (Input_Yaw > 0.f) Input_Yaw = 0.f;
}

void ADronePawn::OnBrake()
{
    if (bRTLMode) return;
    bBraking = true; bLoiterMode = false; bWaypointMode = false;
    DroneVelocity = FVector::ZeroVector;
    Input_Throttle = Input_Pitch = Input_Roll = Input_Yaw = 0.f;
    Target_Throttle = Target_Pitch = Target_Roll = Target_Yaw = 0.f;
    CurrentPitch = CurrentRoll = 0.f;
    FRotator R = GetActorRotation(); R.Pitch = 0.f; R.Roll = 0.f;
    SetActorRotation(R);
    if (GEngine) GEngine->AddOnScreenDebugMessage(7, 1.5f, FColor::Yellow, TEXT("BRAKE APPLIED"));
    GetWorldTimerManager().SetTimer(BrakeTimerHandle, [this]() {bBraking = false; }, 0.1f, false);
}
void ADronePawn::OnClearTrail()
{
    ClearFlightPath();
    if (GEngine) GEngine->AddOnScreenDebugMessage(8, 2.f, FColor::White, TEXT(">> TRAIL CLEARED <<"));
}
void ADronePawn::OnToggleCamera()
{
    bFPVMode = !bFPVMode; UpdateCameras();
    if (GEngine) GEngine->AddOnScreenDebugMessage(6, 2.f, FColor::Cyan,
        bFPVMode ? TEXT(">> FPV MODE <<") : TEXT(">> 3RD PERSON MODE <<"));
}
void ADronePawn::OnToggleInputMode()
{
    bJoystickMode = !bJoystickMode;
    Input_Throttle = Input_Pitch = Input_Roll = Input_Yaw = 0.f;
    Target_Throttle = Target_Pitch = Target_Roll = Target_Yaw = 0.f;
    if (GEngine) GEngine->AddOnScreenDebugMessage(5, 3.f, FColor::Yellow,
        bJoystickMode ? TEXT(">> JOYSTICK MODE <<") : TEXT(">> KEYBOARD MODE <<"));
}