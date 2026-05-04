#include "TerrainManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Engine/Engine.h"

ATerrainManager::ATerrainManager()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.TickInterval = 0.3f;

    //--------------------------------------------------
    // AUTO LOAD TREES
    // From your SeyeonjeongPavilion asset
    //--------------------------------------------------
    static ConstructorHelpers::FObjectFinder
        <UStaticMesh> T1(TEXT(
            "StaticMesh'/Game/SeyeonjeongPavilion/Mesh"
            "/Tree/SM_Aphananthe_Aspera_Fall_1"
            ".SM_Aphananthe_Aspera_Fall_1'"));
    if (T1.Succeeded()) TreeMeshes.Add(T1.Object);

    static ConstructorHelpers::FObjectFinder
        <UStaticMesh> T2(TEXT(
            "StaticMesh'/Game/SeyeonjeongPavilion/Mesh"
            "/Tree/SM_Aphananthe_Aspera_Fall_2"
            ".SM_Aphananthe_Aspera_Fall_2'"));
    if (T2.Succeeded()) TreeMeshes.Add(T2.Object);

    static ConstructorHelpers::FObjectFinder
        <UStaticMesh> T3(TEXT(
            "StaticMesh'/Game/SeyeonjeongPavilion/Mesh"
            "/Tree/SM_Camelliajaponica_1"
            ".SM_Camelliajaponica_1'"));
    if (T3.Succeeded()) TreeMeshes.Add(T3.Object);

    //--------------------------------------------------
    // AUTO LOAD ROCKS
    //--------------------------------------------------
    static ConstructorHelpers::FObjectFinder
        <UStaticMesh> R1(TEXT(
            "StaticMesh'/Game/SeyeonjeongPavilion/Mesh"
            "/Rock/SM_Rock_A.SM_Rock_A'"));
    if (R1.Succeeded()) RockMeshes.Add(R1.Object);

    static ConstructorHelpers::FObjectFinder
        <UStaticMesh> R2(TEXT(
            "StaticMesh'/Game/SeyeonjeongPavilion/Mesh"
            "/Rock/SM_Rock_B.SM_Rock_B'"));
    if (R2.Succeeded()) RockMeshes.Add(R2.Object);

    static ConstructorHelpers::FObjectFinder
        <UStaticMesh> R3(TEXT(
            "StaticMesh'/Game/SeyeonjeongPavilion/Mesh"
            "/Rock/SM_Rock_C.SM_Rock_C'"));
    if (R3.Succeeded()) RockMeshes.Add(R3.Object);

    static ConstructorHelpers::FObjectFinder
        <UStaticMesh> R4(TEXT(
            "StaticMesh'/Game/SeyeonjeongPavilion/Mesh"
            "/Rock/SM_Rock_D.SM_Rock_D'"));
    if (R4.Succeeded()) RockMeshes.Add(R4.Object);

    //--------------------------------------------------
    // AUTO LOAD PLANTS
    //--------------------------------------------------
    static ConstructorHelpers::FObjectFinder
        <UStaticMesh> P1(TEXT(
            "StaticMesh'/Game/SeyeonjeongPavilion/Mesh"
            "/Plant/SM_Grass.SM_Grass'"));
    if (P1.Succeeded()) PlantMeshes.Add(P1.Object);

    static ConstructorHelpers::FObjectFinder
        <UStaticMesh> P2(TEXT(
            "StaticMesh'/Game/SeyeonjeongPavilion/Mesh"
            "/Plant/SM_Carex_1.SM_Carex_1'"));
    if (P2.Succeeded()) PlantMeshes.Add(P2.Object);

    static ConstructorHelpers::FObjectFinder
        <UStaticMesh> P3(TEXT(
            "StaticMesh'/Game/SeyeonjeongPavilion/Mesh"
            "/Plant/SM_Deparia_1.SM_Deparia_1'"));
    if (P3.Succeeded()) PlantMeshes.Add(P3.Object);

    //--------------------------------------------------
    // AUTO LOAD BUILDING
    //--------------------------------------------------
    static ConstructorHelpers::FObjectFinder
        <UStaticMesh> B1(TEXT(
            "StaticMesh'/Game/SeyeonjeongPavilion/Mesh"
            "/Building/SM_Seyeonjeong"
            ".SM_Seyeonjeong'"));
    if (B1.Succeeded()) BuildingMeshes.Add(B1.Object);

    //--------------------------------------------------
    // AUTO LOAD GROUND MATERIAL
    //--------------------------------------------------
    static ConstructorHelpers::FObjectFinder
        <UMaterialInterface> GM(TEXT(
            "MaterialInstanceConstant'/Game"
            "/SeyeonjeongPavilion/Material/Ground"
            "/MI_Ground_1.MI_Ground_1'"));
    if (GM.Succeeded()) TerrainMaterial = GM.Object;

    //--------------------------------------------------
    // AUTO LOAD WATER MATERIAL
    //--------------------------------------------------
    static ConstructorHelpers::FObjectFinder
        <UMaterialInterface> WM(TEXT(
            "MaterialInstanceConstant'/Game"
            "/SeyeonjeongPavilion/Material/Ground"
            "/MI_Water.MI_Water'"));
    if (WM.Succeeded()) WaterMaterial = WM.Object;
}

void ATerrainManager::BeginPlay()
{
    Super::BeginPlay();

    APlayerController* PC =
        UGameplayStatics::GetPlayerController(
            GetWorld(), 0);
    if (PC) PlayerPawn = PC->GetPawn();

    if (GEngine)
        GEngine->AddOnScreenDebugMessage(
            30, 5.f, FColor::Green,
            FString::Printf(TEXT(
                ">> TERRAIN MANAGER READY <<"
                " Trees:%d Rocks:%d Plants:%d"
                " Buildings:%d <<"),
                TreeMeshes.Num(),
                RockMeshes.Num(),
                PlantMeshes.Num(),
                BuildingMeshes.Num()));
}

void ATerrainManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!PlayerPawn)
    {
        APlayerController* PC =
            UGameplayStatics::GetPlayerController(
                GetWorld(), 0);
        if (PC) PlayerPawn = PC->GetPawn();
        return;
    }

    UpdateChunks();
}

FIntPoint ATerrainManager::WorldToChunk(FVector Pos)
{
    return FIntPoint(
        FMath::FloorToInt(Pos.X / ChunkSize),
        FMath::FloorToInt(Pos.Y / ChunkSize));
}

void ATerrainManager::UpdateChunks()
{
    FIntPoint PC = WorldToChunk(
        PlayerPawn->GetActorLocation());

    if (PC == LastPlayerChunk) return;
    LastPlayerChunk = PC;

    // Collect needed chunks
    TSet<FIntPoint> Needed;
    for (int32 X = -ViewDistance;
        X <= ViewDistance; X++)
        for (int32 Y = -ViewDistance;
            Y <= ViewDistance; Y++)
            Needed.Add(FIntPoint(PC.X + X, PC.Y + Y));

    // Load new chunks
    for (const FIntPoint& C : Needed)
        if (!LoadedChunks.Contains(C))
            LoadChunk(C);

    // Unload far chunks
    TArray<FIntPoint> ToRemove;
    for (auto& Pair : LoadedChunks)
    {
        FIntPoint C = Pair.Key;
        int32 DX = FMath::Abs(C.X - PC.X);
        int32 DY = FMath::Abs(C.Y - PC.Y);
        if (DX > ViewDistance + 1 ||
            DY > ViewDistance + 1)
            ToRemove.Add(C);
    }
    for (const FIntPoint& C : ToRemove)
        UnloadChunk(C);
}

void ATerrainManager::LoadChunk(FIntPoint Coord)
{
    FVector Origin(
        Coord.X * ChunkSize,
        Coord.Y * ChunkSize,
        0.f);

    ATerrainChunk* Chunk =
        GetWorld()->SpawnActor<ATerrainChunk>(
            ATerrainChunk::StaticClass(),
            Origin,
            FRotator::ZeroRotator);

    if (!Chunk) return;

    Chunk->GenerateTerrain(
        Coord,
        ChunkSize,
        ChunkResolution,
        HeightScale,
        NoiseScale,
        WaterLevel,
        TerrainMaterial,
        WaterMaterial);

    Chunk->SpawnProps(
        TreeMeshes,
        RockMeshes,
        PlantMeshes,
        BuildingMeshes,
        ChunkSize,
        WaterLevel,
        HeightScale);

    LoadedChunks.Add(Coord, Chunk);

    if (GEngine)
        GEngine->AddOnScreenDebugMessage(
            -1, 1.f, FColor::Cyan,
            FString::Printf(TEXT(
                "CHUNK LOADED [%d,%d]"),
                Coord.X, Coord.Y));
}

void ATerrainManager::UnloadChunk(FIntPoint Coord)
{
    if (!LoadedChunks.Contains(Coord)) return;

    ATerrainChunk* Chunk = LoadedChunks[Coord];
    if (Chunk) Chunk->DestroyChunk();
    LoadedChunks.Remove(Coord);

    if (GEngine)
        GEngine->AddOnScreenDebugMessage(
            -1, 1.f, FColor::Red,
            FString::Printf(TEXT(
                "CHUNK UNLOADED [%d,%d]"),
                Coord.X, Coord.Y));
}