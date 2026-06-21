// PenguinSpawnSubsystem.cpp
#include "PenguinSpawnSubsystem.h"
#include "PenguinBase.h"
#include "PenguinSpawnPoint.h"

bool UPenguinSpawnSubsystem::TrySpawnPenguin(TSubclassOf<APenguinBase> PenguinClass, APenguinSpawnPoint* SpawnPoint)
{
    if (!PenguinClass || !SpawnPoint) return false;
    if (SpawnPoint->IsOccupied()) return false; // slot already taken

    FTransform SpawnTransform = SpawnPoint->GetActorTransform();
    APenguinBase* NewPenguin = GetWorld()->SpawnActor<APenguinBase>(PenguinClass, SpawnTransform);

    if (NewPenguin)
    {
        SpawnPoint->SetOccupant(NewPenguin);
        NewPenguin->OnDestroyed.AddDynamic(SpawnPoint, &APenguinSpawnPoint::OnOccupantDestroyed);
        return true;
    }
    return false;
}