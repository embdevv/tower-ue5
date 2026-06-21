// PenguinSpawnSubsystem.h
#pragma once
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PenguinSpawnSubsystem.generated.h"

class APenguinBase;
class APenguinSpawnPoint;

UCLASS()
class UPenguinSpawnSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category = "Penguins")
    bool TrySpawnPenguin(TSubclassOf<APenguinBase> PenguinClass, APenguinSpawnPoint* SpawnPoint);
};