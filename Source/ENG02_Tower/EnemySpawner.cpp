#include "EnemySpawner.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "TimerManager.h"

AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	InitializePool();
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AEnemySpawner::SpawnEnemy, MinSpawnDelay, false);
}

void AEnemySpawner::InitializePool()
{
	if (!EnemyClassToSpawn) return;

	for (int i = 0; i < InitialPoolSize; i++)
	{
		ATowerEnemyBase* NewEnemy = GetWorld()->SpawnActor<ATowerEnemyBase>(EnemyClassToSpawn, FVector::ZeroVector, FRotator::ZeroRotator);

		if (NewEnemy)
		{
			NewEnemy->OnEnemyReachedBase.AddDynamic(this, &AEnemySpawner::HandleEnemyReachedBase);
			NewEnemy->OnEnemyDied.AddDynamic(this, &AEnemySpawner::HandleEnemyDied);
			NewEnemy->DeactivateEnemy();
			EnemyPool.Add(NewEnemy);
		}
	}
}

ATowerEnemyBase* AEnemySpawner::GetEnemyFromPool()
{
	for (ATowerEnemyBase* Enemy : EnemyPool)
	{
		if (!Enemy->bIsActive) return Enemy;
	}
	return nullptr;
}

void AEnemySpawner::SpawnEnemy()
{
	// LIVE TRACKER
	int32 ActiveCount = 0;
	for (ATowerEnemyBase* E : EnemyPool)
	{
		if (E->bIsActive) ActiveCount++;
	}
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Cyan, FString::Printf(TEXT("Active Enemies: %d / %d"), ActiveCount, InitialPoolSize));
	}

	ATowerEnemyBase* PooledEnemy = GetEnemyFromPool();
	if (PooledEnemy)
	{
		// custom speed variables
		float RandomSpeed = FMath::RandRange(MinEnemySpeed, MaxEnemySpeed);
		PooledEnemy->ActivateEnemy(GetActorLocation(), SpawnerPath, RandomSpeed);
	}

	float NextSpawnTime = FMath::RandRange(MinSpawnDelay, MaxSpawnDelay);
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AEnemySpawner::SpawnEnemy, NextSpawnTime, false);
}

void AEnemySpawner::HandleEnemyReachedBase(float Damage, ATowerEnemyBase* Enemy)
{
	// Prints to screen
	if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("Base took %f damage!"), Damage)); }

	// Prints to Output Log
	UE_LOG(LogTemp, Warning, TEXT("EVENT TRIGGERED: Base took %f damage"), Damage);

	Enemy->DeactivateEnemy(); // Recycle
}

void AEnemySpawner::HandleEnemyDied(ATowerEnemyBase* Enemy)
{
	UE_LOG(LogTemp, Warning, TEXT("EVENT TRIGGERED: Enemy was killed by defender!"));
	Enemy->DeactivateEnemy(); // Recycle
}