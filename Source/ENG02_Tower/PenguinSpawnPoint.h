// PenguinSpawnPoint.h
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PenguinSpawnPoint.generated.h"

class APenguinBase;

UCLASS(Blueprintable)
class APenguinSpawnPoint : public AActor
{
    GENERATED_BODY()
public:
    APenguinSpawnPoint();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class USceneComponent* Root;

    // Visual marker
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    class UStaticMeshComponent* MarkerMesh;

    UPROPERTY(BlueprintReadOnly)
    APenguinBase* OccupyingPenguin = nullptr;

    UFUNCTION(BlueprintCallable, BlueprintPure)
    bool IsOccupied() const { return OccupyingPenguin != nullptr; }

    void SetOccupant(APenguinBase* Penguin) { OccupyingPenguin = Penguin; }

    UFUNCTION()
    void OnOccupantDestroyed(AActor* DestroyedActor);
};