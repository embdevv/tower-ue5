#include "PenguinSpawnSubsystem.h"
#include "PenguinBase.h"
#include "PenguinSpawnPoint.h"
#include "TowerGameMode.h"

bool UPenguinSpawnSubsystem::TrySpawnPenguin(TSubclassOf<APenguinBase> PenguinClass, APenguinSpawnPoint* SpawnPoint)
{
    if (!PenguinClass || !SpawnPoint) return false;
    if (SpawnPoint->IsOccupied()) return false;

    APenguinBase* CDO = PenguinClass->GetDefaultObject<APenguinBase>();
    ATowerGameMode* GM = Cast<ATowerGameMode>(GetWorld()->GetAuthGameMode());
    if (!GM || !GM->SpendCoins(CDO->CoinCost)) return false;

    FTransform SpawnTransform = SpawnPoint->GetActorTransform();

    FVector SpawnLocation = SpawnTransform.GetLocation();
    SpawnLocation.Z += 30.f;
    SpawnTransform.SetLocation(SpawnLocation);

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    APenguinBase* NewPenguin = GetWorld()->SpawnActor<APenguinBase>(PenguinClass, SpawnTransform, SpawnParams);

    if (NewPenguin)
    {
        SpawnPoint->SetOccupant(NewPenguin);
        NewPenguin->OnDestroyed.AddDynamic(SpawnPoint, &APenguinSpawnPoint::OnOccupantDestroyed);
        return true;
    }
    return false;
}