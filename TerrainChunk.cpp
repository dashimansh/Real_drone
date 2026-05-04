#include "TerrainChunk.h"
#include "KismetProceduralMeshLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/Engine.h"

ATerrainChunk::ATerrainChunk()
{
    PrimaryActorTick.bCanEverTick = false;

    TerrainMesh =
        CreateDefaultSubobject<UProceduralMeshComponent>(
            TEXT("TerrainMesh"));
    RootComponent = TerrainMesh;
    TerrainMesh->SetCastShadow(true);
    TerrainMesh->SetCollisionEnabled(
        ECollisionEnabled::QueryAndPhysics);

    WaterMesh =
        CreateDefaultSubobject<UProceduralMeshComponent>(
            TEXT("WaterMesh"));
    WaterMesh->SetupAttachment(RootComponent);
    WaterMesh->SetCastShadow(false);
    WaterMesh->SetCollisionEnabled(
        ECollisionEnabled::NoCollision);
}

void ATerrainChunk::GenerateTerrain(
    FIntPoint           Coord,
    int32               ChunkSize,
    int32               Resolution,
    float               HeightScale,
    float               NoiseScale,
    float               WaterLevel,
    UMaterialInterface* TerrainMat,
    UMaterialInterface* WaterMat)
{
    ChunkCoord = Coord;
    CachedChunkSize = ChunkSize;
    CachedNoiseScale = NoiseScale;
    CachedHeightScale = HeightScale;
    CachedWaterLevel = WaterLevel;

    float OriginX = Coord.X * ChunkSize;
    float OriginY = Coord.Y * ChunkSize;
    float Step = (float)ChunkSize / Resolution;

    TArray<FVector>          Verts;
    TArray<int32>            Tris;
    TArray<FVector>          Normals;
    TArray<FVector2D>        UVs;
    TArray<FColor>           Colors;
    TArray<FProcMeshTangent> Tangents;

    //--------------------------------------------------
    // GENERATE VERTICES
    //--------------------------------------------------
    for (int32 Y = 0; Y <= Resolution; Y++)
        for (int32 X = 0; X <= Resolution; X++)
        {
            float WX = OriginX + X * Step;
            float WY = OriginY + Y * Step;
            float WZ = SampleHeight(WX, WY,
                NoiseScale, HeightScale);

            Verts.Add(FVector(X * Step, Y * Step, WZ));
            UVs.Add(FVector2D(
                (float)X / Resolution,
                (float)Y / Resolution));

            float HN = FMath::Clamp(
                WZ / HeightScale, 0.f, 1.f);
            FColor VC;
            if (WZ < WaterLevel + 30.f)
                VC = FColor(210, 180, 140); // sand
            else if (HN < 0.35f)
                VC = FColor(34, 139, 34);   // grass
            else if (HN < 0.65f)
                VC = FColor(101, 67, 33);   // dirt
            else if (HN < 0.80f)
                VC = FColor(128, 128, 128); // rock
            else
                VC = FColor(240, 240, 255); // snow
            Colors.Add(VC);
        }

    //--------------------------------------------------
    // GENERATE TRIANGLES
    //--------------------------------------------------
    int32 Stride = Resolution + 1;
    for (int32 Y = 0; Y < Resolution; Y++)
        for (int32 X = 0; X < Resolution; X++)
        {
            int32 I = X + Y * Stride;
            int32 I1 = I + 1;
            int32 I2 = I + Stride;
            int32 I3 = I + Stride + 1;

            Tris.Add(I);  Tris.Add(I2); Tris.Add(I1);
            Tris.Add(I1); Tris.Add(I2); Tris.Add(I3);
        }

    //--------------------------------------------------
    // CALCULATE NORMALS
    //--------------------------------------------------
    UKismetProceduralMeshLibrary::CalculateTangentsForMesh(
        Verts, Tris, UVs, Normals, Tangents);

    //--------------------------------------------------
    // CREATE TERRAIN MESH
    //--------------------------------------------------
    TerrainMesh->CreateMeshSection(
        0, Verts, Tris, Normals,
        UVs, Colors, Tangents, true);

    if (TerrainMat)
        TerrainMesh->SetMaterial(0, TerrainMat);

    //--------------------------------------------------
    // CREATE WATER PLANE
    //--------------------------------------------------
    TArray<FVector>   WV;
    TArray<int32>     WT;
    TArray<FVector>   WN;
    TArray<FVector2D> WUV;
    TArray<FProcMeshTangent> WTangents;

    float WZ = WaterLevel;
    WV.Add(FVector(0, 0, WZ));
    WV.Add(FVector(ChunkSize, 0, WZ));
    WV.Add(FVector(ChunkSize, ChunkSize, WZ));
    WV.Add(FVector(0, ChunkSize, WZ));

    WUV.Add(FVector2D(0, 0));
    WUV.Add(FVector2D(1, 0));
    WUV.Add(FVector2D(1, 1));
    WUV.Add(FVector2D(0, 1));

    WN.Add(FVector(0, 0, 1));
    WN.Add(FVector(0, 0, 1));
    WN.Add(FVector(0, 0, 1));
    WN.Add(FVector(0, 0, 1));

    WT.Add(0); WT.Add(1); WT.Add(2);
    WT.Add(0); WT.Add(2); WT.Add(3);

    WaterMesh->CreateMeshSection(
        0, WV, WT, WN, WUV,
        TArray<FColor>(), WTangents, false);

    if (WaterMat)
        WaterMesh->SetMaterial(0, WaterMat);

    bIsGenerated = true;
}

void ATerrainChunk::SpawnProps(
    TArray<UStaticMesh*> TreeMeshes,
    TArray<UStaticMesh*> RockMeshes,
    TArray<UStaticMesh*> PlantMeshes,
    TArray<UStaticMesh*> BuildingMeshes,
    int32 ChunkSize,
    float WaterLevel,
    float HeightScale)
{
    float OX = ChunkCoord.X * ChunkSize;
    float OY = ChunkCoord.Y * ChunkSize;

    //--------------------------------------------------
    // SPAWN TREES
    //--------------------------------------------------
    if (TreeMeshes.Num() > 0)
    {
        FRandomStream R(
            ChunkCoord.X * 1000 + ChunkCoord.Y);
        for (int32 i = 0; i < 15; i++)
        {
            float TX = OX + R.FRandRange(
                300.f, ChunkSize - 300.f);
            float TY = OY + R.FRandRange(
                300.f, ChunkSize - 300.f);
            float TZ = SampleHeight(TX, TY,
                CachedNoiseScale, CachedHeightScale);

            if (TZ < WaterLevel + 100.f) continue;
            if (TZ / HeightScale > 0.72f) continue;

            UStaticMesh* M = TreeMeshes[
                R.RandRange(0, TreeMeshes.Num() - 1)];
            if (!M) continue;

            SpawnMesh(M,
                FVector(TX, TY, TZ),
                FRotator(0.f,
                    R.FRandRange(0.f, 360.f), 0.f),
                R.FRandRange(0.8f, 1.3f));
        }
    }

    //--------------------------------------------------
    // SPAWN ROCKS
    //--------------------------------------------------
    if (RockMeshes.Num() > 0)
    {
        FRandomStream R(
            ChunkCoord.X * 2000 + ChunkCoord.Y * 3);
        for (int32 i = 0; i < 10; i++)
        {
            float RX = OX + R.FRandRange(
                100.f, ChunkSize - 100.f);
            float RY = OY + R.FRandRange(
                100.f, ChunkSize - 100.f);
            float RZ = SampleHeight(RX, RY,
                CachedNoiseScale, CachedHeightScale);

            if (RZ < WaterLevel) continue;

            UStaticMesh* M = RockMeshes[
                R.RandRange(0, RockMeshes.Num() - 1)];
            if (!M) continue;

            SpawnMesh(M,
                FVector(RX, RY, RZ),
                FRotator(0.f,
                    R.FRandRange(0.f, 360.f), 0.f),
                R.FRandRange(0.5f, 2.f));
        }
    }

    //--------------------------------------------------
    // SPAWN PLANTS
    //--------------------------------------------------
    if (PlantMeshes.Num() > 0)
    {
        FRandomStream R(
            ChunkCoord.X * 3000 + ChunkCoord.Y * 7);
        for (int32 i = 0; i < 20; i++)
        {
            float PX = OX + R.FRandRange(
                50.f, ChunkSize - 50.f);
            float PY = OY + R.FRandRange(
                50.f, ChunkSize - 50.f);
            float PZ = SampleHeight(PX, PY,
                CachedNoiseScale, CachedHeightScale);

            if (PZ < WaterLevel + 50.f) continue;
            if (PZ / HeightScale > 0.6f) continue;

            UStaticMesh* M = PlantMeshes[
                R.RandRange(0, PlantMeshes.Num() - 1)];
            if (!M) continue;

            SpawnMesh(M,
                FVector(PX, PY, PZ),
                FRotator(0.f,
                    R.FRandRange(0.f, 360.f), 0.f),
                R.FRandRange(0.7f, 1.2f));
        }
    }

    //--------------------------------------------------
    // SPAWN BUILDING (1 in every 5 chunks)
    //--------------------------------------------------
    if (BuildingMeshes.Num() > 0)
    {
        FRandomStream R(
            ChunkCoord.X * 5000 + ChunkCoord.Y * 11);

        if (R.RandRange(0, 4) == 0)
        {
            float BX = OX + ChunkSize * 0.5f;
            float BY = OY + ChunkSize * 0.5f;
            float BZ = SampleHeight(BX, BY,
                CachedNoiseScale, CachedHeightScale);

            if (BZ > WaterLevel + 100.f &&
                BZ / HeightScale < 0.4f)
            {
                UStaticMesh* M = BuildingMeshes[
                    R.RandRange(0,
                        BuildingMeshes.Num() - 1)];
                if (M)
                    SpawnMesh(M,
                        FVector(BX, BY, BZ),
                        FRotator(0.f,
                            R.FRandRange(0.f, 360.f),
                            0.f),
                        1.0f);
            }
        }
    }
}

void ATerrainChunk::SpawnMesh(
    UStaticMesh* Mesh,
    FVector      Location,
    FRotator     Rotation,
    float        Scale)
{
    if (!Mesh || !GetWorld()) return;

    FActorSpawnParameters SP;
    SP.SpawnCollisionHandlingOverride =
        ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AActor* Actor = GetWorld()->SpawnActor<AActor>(
        AActor::StaticClass(),
        Location, Rotation, SP);
    if (!Actor) return;

    UStaticMeshComponent* SMC =
        NewObject<UStaticMeshComponent>(Actor);
    SMC->SetStaticMesh(Mesh);
    SMC->RegisterComponent();
    SMC->SetRelativeScale3D(FVector(Scale));
    Actor->SetRootComponent(SMC);
    SpawnedActors.Add(Actor);
}

void ATerrainChunk::DestroyChunk()
{
    for (AActor* A : SpawnedActors)
        if (A) A->Destroy();
    SpawnedActors.Empty();
    Destroy();
}

float ATerrainChunk::SampleHeight(
    float X, float Y,
    float NoiseScale,
    float HeightScale)
{
    float H = 0.f, A = 1.f;
    float F = NoiseScale, Max = 0.f;

    for (int32 O = 0; O < 6; O++)
    {
        H += PerlinNoise(X * F, Y * F) * A;
        Max += A;
        A *= 0.5f;
        F *= 2.1f;
    }

    H = (H / Max + 1.f) * 0.5f;
    H = FMath::Pow(H, 1.8f);
    H *= HeightScale;
    return FMath::Max(H, 0.f);
}

float ATerrainChunk::PerlinNoise(float X, float Y)
{
    static const int P[] = {
        151,160,137,91,90,15,131,13,201,95,96,53,
        194,233,7,225,140,36,103,30,69,142,8,99,37,
        240,21,10,23,190,6,148,247,120,234,75,0,26,
        197,62,94,252,219,203,117,35,11,32,57,177,
        33,88,237,149,56,87,174,20,125,136,171,168,
        68,175,74,165,71,134,139,48,27,166,77,146,
        158,231,83,111,229,122,60,211,133,230,220,
        105,92,41,55,46,245,40,244,102,143,54,65,25,
        63,161,1,216,80,73,209,76,132,187,208,89,18,
        169,200,196,135,130,116,188,159,86,164,100,
        109,198,173,186,3,64,52,217,226,250,124,123,
        5,202,38,147,118,126,255,82,85,212,207,206,
        59,227,47,16,58,17,182,189,28,42,223,183,170,
        213,119,248,152,2,44,154,163,70,221,153,101,
        155,167,43,172,9,129,22,39,253,19,98,108,110,
        79,113,224,232,178,185,112,104,218,246,97,228,
        251,34,242,193,238,210,144,12,191,179,162,241,
        81,51,145,235,249,14,239,107,49,192,214,31,
        181,199,106,157,184,84,204,176,115,121,50,45,
        127,4,150,254,138,236,205,93,222,114,67,29,24,
        72,243,141,128,195,78,66,215,61,156,180,
        // repeat
        151,160,137,91,90,15,131,13,201,95,96,53,
        194,233,7,225,140,36,103,30,69,142,8,99,37,
        240,21,10,23,190,6,148,247,120,234,75,0,26,
        197,62,94,252,219,203,117,35,11,32,57,177,
        33,88,237,149,56,87,174,20,125,136,171,168,
        68,175,74,165,71,134,139,48,27,166,77,146,
        158,231,83,111,229,122,60,211,133,230,220,
        105,92,41,55,46,245,40,244,102,143,54,65,25,
        63,161,1,216,80,73,209,76,132,187,208,89,18,
        169,200,196,135,130,116,188,159,86,164,100,
        109,198,173,186,3,64,52,217,226,250,124,123,
        5,202,38,147,118,126,255,82,85,212,207,206,
        59,227,47,16,58,17,182,189,28,42,223,183,170,
        213,119,248,152,2,44,154,163,70,221,153,101,
        155,167,43,172,9,129,22,39,253,19,98,108,110,
        79,113,224,232,178,185,112,104,218,246,97,228,
        251,34,242,193,238,210,144,12,191,179,162,241,
        81,51,145,235,249,14,239,107,49,192,214,31,
        181,199,106,157,184,84,204,176,115,121,50,45,
        127,4,150,254,138,236,205,93,222,114,67,29,24,
        72,243,141,128,195,78,66,215,61,156,180
    };

    int IX = FMath::FloorToInt(X) & 255;
    int IY = FMath::FloorToInt(Y) & 255;
    float FX = X - FMath::FloorToInt(X);
    float FY = Y - FMath::FloorToInt(Y);
    float U = Fade(FX);
    float V = Fade(FY);

    int A = P[IX] + IY;
    int B = P[IX + 1] + IY;

    return LerpF(
        LerpF(Grad(P[A], FX, FY),
            Grad(P[B], FX - 1, FY), U),
        LerpF(Grad(P[A + 1], FX, FY - 1),
            Grad(P[B + 1], FX - 1, FY - 1), U),
        V);
}

float ATerrainChunk::Fade(float T)
{
    return T * T * T * (T * (T * 6 - 15) + 10);
}

float ATerrainChunk::LerpF(float A, float B, float T)
{
    return A + T * (B - A);
}

float ATerrainChunk::Grad(int H, float X, float Y)
{
    int   HH = H & 3;
    float U = HH < 2 ? X : Y;
    float V = HH < 2 ? Y : X;
    return ((HH & 1) ? -U : U) + ((HH & 2) ? -V : V);
}