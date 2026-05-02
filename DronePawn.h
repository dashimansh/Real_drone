#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DronePawn.generated.h"

class ADroneHUD;

UCLASS()
class DRONE_PACK_API ADronePawn : public APawn
{
    GENERATED_BODY()

public:
    ADronePawn();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(
        class UInputComponent* PlayerInputComponent) override;

    //----------------------------------------------------
    // MESH COMPONENTS
    //----------------------------------------------------
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drone|Mesh")
        class USkeletalMeshComponent* BodyMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drone|Mesh")
        class USceneComponent* FL_PropellerRoot;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drone|Mesh")
        class USceneComponent* FL_PropellerPivot;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drone|Mesh")
        class USkeletalMeshComponent* FL_PropellerMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drone|Mesh")
        class USceneComponent* FR_PropellerRoot;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drone|Mesh")
        class USceneComponent* FR_PropellerPivot;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drone|Mesh")
        class USkeletalMeshComponent* FR_PropellerMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drone|Mesh")
        class USceneComponent* BL_PropellerRoot;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drone|Mesh")
        class USceneComponent* BL_PropellerPivot;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drone|Mesh")
        class USkeletalMeshComponent* BL_PropellerMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drone|Mesh")
        class USceneComponent* BR_PropellerRoot;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drone|Mesh")
        class USceneComponent* BR_PropellerPivot;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drone|Mesh")
        class USkeletalMeshComponent* BR_PropellerMesh;

    //----------------------------------------------------
    // PHYSICS ROOT
    //----------------------------------------------------
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drone|Physics")
        class UStaticMeshComponent* PhysicsBody;

    //----------------------------------------------------
    // CAMERAS
    //----------------------------------------------------
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drone|Camera")
        class USpringArmComponent* SpringArm;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drone|Camera")
        class UCameraComponent* ThirdPersonCamera;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drone|Camera")
        class UCameraComponent* FPVCamera;

    //----------------------------------------------------
    // PID TUNING
    //----------------------------------------------------
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone|PID")
        float Alt_Kp = 15.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone|PID")
        float Alt_Ki = 0.05f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone|PID")
        float Alt_Kd = 5.f;

    //----------------------------------------------------
    // PHYSICS SETTINGS
    //----------------------------------------------------
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone|Physics")
        float Mass = 1.5f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone|Physics")
        float MaxRotorThrust = 2400.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone|Physics")
        float MaxRotorSpeed = 3000.f;

    //----------------------------------------------------
    // MOVEMENT SETTINGS
    //----------------------------------------------------
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone|Feel")
        float ThrottleSpeed = 800.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone|Feel")
        float PitchSpeed = 300.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone|Feel")
        float RollSpeed = 300.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone|Feel")
        float YawSpeed = 120.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone|Feel")
        float MaxTiltAngle = 45.f;

    //----------------------------------------------------
    // PROPELLER SETTINGS
    //----------------------------------------------------
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone|Propellers")
        float PropellerSmoothSpeed = 5.f;

    //----------------------------------------------------
    // FLIGHT PATH TRAIL
    //----------------------------------------------------
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone|Trail")
        float PathPointDistance = 50.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone|Trail")
        int32 MaxPathPoints = 500;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone|Trail")
        float TrailLineThickness = 3.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone|Trail")
        bool bShowTrail = true;

    //----------------------------------------------------
    // LOITER AND RTL MODE
    //----------------------------------------------------
    UPROPERTY(BlueprintReadOnly, Category = "Drone|Mode")
        bool bLoiterMode = false;
    UPROPERTY(BlueprintReadOnly, Category = "Drone|Mode")
        bool bRTLMode = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone|Mode")
        float RTLSpeed = 600.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone|Mode")
        float RTLAcceptRadius = 150.f;

    //----------------------------------------------------
    // WAYPOINT NAVIGATION
    //----------------------------------------------------
    UPROPERTY(BlueprintReadOnly, Category = "Drone|Waypoint")
        bool bWaypointMode = false;

    UPROPERTY(BlueprintReadOnly, Category = "Drone|Waypoint")
        int32 CurrentWaypointIndex = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone|Waypoint")
        float WaypointAcceptRadius = 200.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone|Waypoint")
        float WaypointSpeed = 500.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone|Waypoint")
        float WaypointAltitude = 500.f;

    TArray<FVector> Waypoints;

    //----------------------------------------------------
    // MOTOR THRUST VALUES
    //----------------------------------------------------
    UPROPERTY(BlueprintReadOnly, Category = "Drone|Motors")
        float Motor_FL = 0.f;
    UPROPERTY(BlueprintReadOnly, Category = "Drone|Motors")
        float Motor_FR = 0.f;
    UPROPERTY(BlueprintReadOnly, Category = "Drone|Motors")
        float Motor_BL = 0.f;
    UPROPERTY(BlueprintReadOnly, Category = "Drone|Motors")
        float Motor_BR = 0.f;

    //----------------------------------------------------
    // FLIGHT DATA
    //----------------------------------------------------
    UPROPERTY(BlueprintReadOnly, Category = "Drone|Flight")
        float CurrentAltitude = 0.f;

    //----------------------------------------------------
    // INPUT AXES
    //----------------------------------------------------
    UPROPERTY(BlueprintReadOnly, Category = "Drone|Input")
        float Input_Throttle = 0.f;
    UPROPERTY(BlueprintReadOnly, Category = "Drone|Input")
        float Input_Pitch = 0.f;
    UPROPERTY(BlueprintReadOnly, Category = "Drone|Input")
        float Input_Roll = 0.f;
    UPROPERTY(BlueprintReadOnly, Category = "Drone|Input")
        float Input_Yaw = 0.f;

    //----------------------------------------------------
    // MODE FLAGS
    //----------------------------------------------------
    UPROPERTY(BlueprintReadOnly, Category = "Drone|Input")
        bool bJoystickMode = false;
    UPROPERTY(BlueprintReadOnly, Category = "Drone|Input")
        bool bBraking = false;
    bool bFPVMode = false;

    //----------------------------------------------------
    // HUD REFERENCE
    //----------------------------------------------------
    UPROPERTY()
        ADroneHUD* DroneHUD;

private:
    //----------------------------------------------------
    // PID
    //----------------------------------------------------
    struct FPIDState
    {
        float Integral = 0.f;
        float PrevError = 0.f;
        void Reset() { Integral = 0.f; PrevError = 0.f; }
    };
    FPIDState PID_Alt;
    float ComputePID(FPIDState& State,
        float Kp, float Ki, float Kd,
        float Error, float DeltaTime);

    //----------------------------------------------------
    // PHYSICS STATE
    //----------------------------------------------------
    FVector DroneVelocity = FVector::ZeroVector;
    float   CurrentPitch = 0.f;
    float   CurrentRoll = 0.f;
    float   TargetAltitude = 0.f;

    //----------------------------------------------------
    // INPUT TARGETS
    //----------------------------------------------------
    float Target_Throttle = 0.f;
    float Target_Pitch = 0.f;
    float Target_Roll = 0.f;
    float Target_Yaw = 0.f;

    //----------------------------------------------------
    // SMOOTH PROPELLER MOTOR VALUES
    //----------------------------------------------------
    float SmoothMotor_FL = 0.f;
    float SmoothMotor_FR = 0.f;
    float SmoothMotor_BL = 0.f;
    float SmoothMotor_BR = 0.f;

    //----------------------------------------------------
    // FLIGHT PATH DATA
    //----------------------------------------------------
    TArray<FVector> FlightPath;
    FVector LastRecordedPos = FVector::ZeroVector;

    //----------------------------------------------------
    // LOITER AND RTL INTERNAL STATE
    //----------------------------------------------------
    FVector LoiterPosition = FVector::ZeroVector;
    FVector HomePosition = FVector::ZeroVector;
    bool    bRTLClimbing = false;
    bool    bRTLReturning = false;
    bool    bRTLHovering = false;

    //----------------------------------------------------
    // BRAKE TIMER
    //----------------------------------------------------
    FTimerHandle BrakeTimerHandle;

    //----------------------------------------------------
    // INTERNAL FUNCTIONS
    //----------------------------------------------------
    void ApplyManualFlight(float DeltaTime);
    void ApplyLoiterFlight(float DeltaTime);
    void ApplyRTLFlight(float DeltaTime);
    void ApplyWaypointFlight(float DeltaTime);
    void UpdateMotorValues();
    void SpinPropellers(float DeltaTime);
    void UpdateHUD();
    void UpdateCameras();
    void RecordFlightPath();
    void DrawFlightTrail();
    void ClearFlightPath();
    void DrawWaypoints();

    //----------------------------------------------------
    // INPUT CALLBACKS
    //----------------------------------------------------
    void OnThrottleUp(float Value);
    void OnThrottleDown(float Value);
    void OnPitch(float Value);
    void OnRoll(float Value);
    void OnYawLeft();
    void OnYawLeftReleased();
    void OnYawRight();
    void OnYawRightReleased();
    void OnKeyThrottleUp(float Value);
    void OnKeyThrottleDown(float Value);
    void OnKeyPitch(float Value);
    void OnKeyRoll(float Value);
    void OnKeyYawLeft();
    void OnKeyYawLeftReleased();
    void OnKeyYawRight();
    void OnKeyYawRightReleased();
    void OnToggleCamera();
    void OnToggleInputMode();
    void OnBrake();
    void OnClearTrail();
    void OnLoiterToggle();
    void OnRTLToggle();
    void OnAddWaypoint();
    void OnStartWaypoint();
    void OnClearWaypoints();

    //----------------------------------------------------
    // PROPELLER ANGLES
    //----------------------------------------------------
    float PropAngle_FL = 0.f;
    float PropAngle_FR = 0.f;
    float PropAngle_BL = 0.f;
    float PropAngle_BR = 0.f;
};