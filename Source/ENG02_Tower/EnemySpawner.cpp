#include "EnemySpawner.h"
#include "Engine/World.h"
#include "TimerManager.h"

AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	// First spawn in 2 seconds
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AEnemySpawner::SpawnEnemy, 2.0f, false);
}

void AEnemySpawner::SpawnEnemy()
{
	if (EnemyClassToSpawn)
	{
		ATowerEnemyBase* NewEnemy = GetWorld()->SpawnActor<ATowerEnemyBase>(EnemyClassToSpawn, GetActorLocation(), GetActorRotation());

		if (NewEnemy)
		{
			// Give enemy the path and random speed
			NewEnemy->PathToFollow = SpawnerPath;
			NewEnemy->MoveSpeed = FMath::RandRange(100.0f, 300.0f);
		}
	}

	// Calculate random time for next spawn (between 1.5 - 4 seconds)
	float NextSpawnTime = FMath::RandRange(1.5f, 4.0f);
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AEnemySpawner::SpawnEnemy, NextSpawnTime, false);
}