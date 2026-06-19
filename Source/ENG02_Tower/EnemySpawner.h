#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TowerEnemyBase.h"
#include "EnemySpawner.generated.h"

UCLASS()
class ENG02_TOWER_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
public:
	AEnemySpawner();
protected:
	virtual void BeginPlay() override;
public:
	// The path this spawner gives to the enemies
	UPROPERTY(EditAnywhere, Category = "Spawner Settings")
	TArray<AActor*> SpawnerPath;

	// What enemy to spawn
	UPROPERTY(EditAnywhere, Category = "Spawner Settings")
	TSubclassOf<ATowerEnemyBase> EnemyClassToSpawn;

	void SpawnEnemy();

	FTimerHandle SpawnTimerHandle;
};