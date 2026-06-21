#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TowerEnemyBase.h"
#include "EnemySpawner.generated.h"

// Forward declaration for the UI Widget
class UUserWidget;

UCLASS()
class ENG02_TOWER_API AEnemySpawner : public AActor
{
	GENERATED_BODY()

public:
	AEnemySpawner();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:
	// The type of enemy to spawn
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<ATowerEnemyBase> EnemyToSpawn;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TArray<AActor*> WaypointPath;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	int32 PoolSize = 10;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	float SpawnInterval = 2.0f;

	TArray<ATowerEnemyBase*> EnemyPool;

	void SpawnEnemy();

	UFUNCTION()
	void HandleEnemyReachedBase(float Damage, ATowerEnemyBase* Enemy);

	
	UFUNCTION()
	void HandleEnemyDied(ATowerEnemyBase* Enemy, FVector DeathLocation);

	FTimerHandle SpawnTimerHandle;

	// The slot for your UI Blueprint
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> CoinWidgetClass;
};