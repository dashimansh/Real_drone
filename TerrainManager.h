#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TerrainChunk.h"
#include "TerrainManager.generated.h"

UCLASS()
class DRONE_PACK_API ATerrainManager : public AActor
{
    GENERATED_BODY()

public:
    ATerrainManager();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    //----------------------------------------------------
    // CHUNK SETTINGS
    //----------------------------------------------------
    UPROPERTY(EditAnywhere, Category = "Terrain|Chunks")
    int32 ChunkSize = 10000;

    UPROPERTY(EditAnywhere, Category = "Terrain|Chunks")
    int32 ViewDistance = 3;

    UPROPERTY(EditAnywhere, Category = "Terrain|Chunks")
    int32 ChunkResolution = 32;

    //----------------------------------------------------
    // TERRAIN SETTINGS
    //----------------------------------------------------
    UPROPERTY(EditAnywhere, Category = "Terrain|Height")
    float HeightScale = 2000.f;

    UPROPERTY(EditAnywhere, Category = "Terrain|Height")
    float NoiseScale = 0.0003f;

    UPROPERTY(EditAnywhere, Category = "Terrain|Height")
    float WaterLevel = 150.f;

    //----------------------------------------------------
    // ASSETS — Auto loaded from your project
    //----------------------------------------------------
    UPROPERTY(EditAnywhere, Category = "Terrain|Assets")
    TArray<UStaticMesh*> TreeMeshes;

    UPROPERTY(EditAnywhere, Category = "Terrain|Assets")
    TArray<UStaticMesh*> RockMeshes;

    UPROPERTY(EditAnywhere, Category = "Terrain|Assets")
    TArray<UStaticMesh*> PlantMeshes;

    UPROPERTY(EditAnywhere, Category = "Terrain|Assets")
    TArray<UStaticMesh*> BuildingMeshes;

    UPROPERTY(EditAnywhere, Category = "Terrain|Assets")
    UMaterialInterface* TerrainMaterial = nullptr;

    UPROPERTY(EditAnywhere, Category = "Terrain|Assets")
    UMaterialInterface* WaterMaterial = nullptr;

private:
    TMap<FIntPoint, ATerrainChunk*> LoadedChunks;
    FIntPoint LastPlayerChunk =
        FIntPoint(-9999, -9999);
    APawn* PlayerPawn = nullptr;

    FIntPoint WorldToChunk(FVector Pos);
    void      UpdateChunks();
    void      LoadChunk(FIntPoint Coord);
    void      UnloadChunk(FIntPoint Coord);
};