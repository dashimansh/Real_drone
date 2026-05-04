#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "TerrainChunk.generated.h"

UCLASS()
class DRONE_PACK_API ATerrainChunk : public AActor
{
    GENERATED_BODY()

public:
    ATerrainChunk();

    //----------------------------------------------------
    // COMPONENTS
    //----------------------------------------------------
    UPROPERTY(VisibleAnywhere)
    UProceduralMeshComponent* TerrainMesh;

    UPROPERTY(VisibleAnywhere)
    UProceduralMeshComponent* WaterMesh;

    //----------------------------------------------------
    // CHUNK DATA
    //----------------------------------------------------
    FIntPoint ChunkCoord;
    bool      bIsGenerated = false;
    int32     CachedChunkSize = 10000;
    float     CachedNoiseScale = 0.0003f;
    float     CachedHeightScale = 2000.f;
    float     CachedWaterLevel = 150.f;

    TArray<AActor*> SpawnedActors;

    //----------------------------------------------------
    // PUBLIC FUNCTIONS
    //----------------------------------------------------
    void GenerateTerrain(
        FIntPoint           Coord,
        int32               ChunkSize,
        int32               Resolution,
        float               HeightScale,
        float               NoiseScale,
        float               WaterLevel,
        UMaterialInterface* TerrainMat,
        UMaterialInterface* WaterMat);

    void SpawnProps(
        TArray<UStaticMesh*> TreeMeshes,
        TArray<UStaticMesh*> RockMeshes,
        TArray<UStaticMesh*> PlantMeshes,
        TArray<UStaticMesh*> BuildingMeshes,
        int32 ChunkSize,
        float WaterLevel,
        float HeightScale);

    void DestroyChunk();

private:
    void SpawnMesh(
        UStaticMesh* Mesh,
        FVector      Location,
        FRotator     Rotation,
        float        Scale);

    float SampleHeight(
        float X, float Y,
        float NoiseScale,
        float HeightScale);

    float PerlinNoise(float X, float Y);
    float Fade(float T);
    float LerpF(float A, float B, float T);
    float Grad(int H, float X, float Y);
};