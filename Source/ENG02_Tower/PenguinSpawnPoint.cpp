// PenguinSpawnPoint.cpp
#include "PenguinSpawnPoint.h"
#include "PenguinBase.h"

APenguinSpawnPoint::APenguinSpawnPoint()
{
    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = Root;

    MarkerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MarkerMesh"));
    MarkerMesh->SetupAttachment(Root);
}

void APenguinSpawnPoint::OnOccupantDestroyed(AActor* DestroyedActor)
{
    OccupyingPenguin = nullptr;
}