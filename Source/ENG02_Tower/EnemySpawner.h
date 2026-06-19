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
	UPROPERTY(EditAnywhere, Category = "Spawner Settings")
	TArray<AActor*> SpawnerPath;

	UPROPERTY(EditAnywhere, Category = "Spawner Settings")
	TSubclassOf<ATowerEnemyBase> EnemyClassToSpawn;

	// Configuarable balancing in details panel 
	// spawn delay and enemy speed can be randomized between the min and max values for each spawn
	UPROPERTY(EditAnywhere, Category = "Spawner Settings|Balancing")
	float MinSpawnDelay = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Spawner Settings|Balancing")
	float MaxSpawnDelay = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Spawner Settings|Balancing")
	float MinEnemySpeed = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Spawner Settings|Balancing")
	float MaxEnemySpeed = 300.0f;
	// ------------------------------

	// POOLING
	UPROPERTY(EditAnywhere, Category = "Spawner Settings|Pooling")
	int32 InitialPoolSize = 20;

	UPROPERTY()
	TArray<ATowerEnemyBase*> EnemyPool;

	void InitializePool();
	ATowerEnemyBase* GetEnemyFromPool();
	void SpawnEnemy();
	FTimerHandle SpawnTimerHandle;

	// EVENT LISTENERS
	UFUNCTION()
	void HandleEnemyReachedBase(float Damage, ATowerEnemyBase* Enemy);

	UFUNCTION()
	void HandleEnemyDied(ATowerEnemyBase* Enemy);
};