#include "EnemySpawner.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "TowerGameMode.h"

AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	if (EnemyToSpawn)
	{
		for (int32 i = 0; i < PoolSize; i++)
		{
			ATowerEnemyBase* NewEnemy = GetWorld()->SpawnActor<ATowerEnemyBase>(EnemyToSpawn, FVector::ZeroVector, FRotator::ZeroRotator);
			if (NewEnemy)
			{
				NewEnemy->DeactivateEnemy();
				NewEnemy->OnEnemyReachedBase.AddDynamic(this, &AEnemySpawner::HandleEnemyReachedBase);

				// Binding the new 2-parameter event
				NewEnemy->OnEnemyDied.AddDynamic(this, &AEnemySpawner::HandleEnemyDied);

				EnemyPool.Add(NewEnemy);
			}
		}
	}

	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AEnemySpawner::SpawnEnemy, SpawnInterval, true);
}

void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemySpawner::SpawnEnemy()
{
	for (ATowerEnemyBase* Enemy : EnemyPool)
	{
		if (!Enemy->bIsActive)
		{
			float RandomSpeed = FMath::RandRange(100.0f, 250.0f);
			Enemy->ActivateEnemy(GetActorLocation(), WaypointPath, RandomSpeed);
			return;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Enemy Pool is empty. Consider increasing PoolSize."));
}

void AEnemySpawner::HandleEnemyReachedBase(float Damage, ATowerEnemyBase* Enemy)
{
	//Get the GameMode
	ATowerGameMode* GM = Cast<ATowerGameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		//Deal the damage
		UE_LOG(LogTemp, Warning, TEXT("Cast succeeded, dealing damage"));
		GM->TakePlayerDamage(Damage);
	}
	else 
	{
		UE_LOG(LogTemp, Error, TEXT("Cast to TowerGameMode FAILED"));
	}

	//Recycle the enemy
	Enemy->DeactivateEnemy();
}

void AEnemySpawner::HandleEnemyDied(ATowerEnemyBase* Enemy, FVector DeathLocation)
{
	UE_LOG(LogTemp, Warning, TEXT("EVENT TRIGGERED: Enemy was killed by defender!"));
	Enemy->DeactivateEnemy(); // Recycle back to the pool

	// The Floating Coin UI Logic
	if (CoinWidgetClass)
	{
		UUserWidget* CoinPopup = CreateWidget<UUserWidget>(GetWorld(), CoinWidgetClass);
		if (CoinPopup)
		{
			CoinPopup->AddToViewport();

			APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			FVector2D ScreenPos;

			// Convert the 3D death location to 2D screen coordinates
			if (PC && PC->ProjectWorldLocationToScreen(DeathLocation, ScreenPos))
			{
				CoinPopup->SetPositionInViewport(ScreenPos);
			}
		}
	}
}