#include "TowerEnemyBase.h"
#include "TargetWaypoint.h" 
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "TowerGameMode.h" //Includes the Bank system. switch to this for main scene when you want to see the coin updates in action

// Sets default values
ATowerEnemyBase::ATowerEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;
	EnemyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnemyMesh"));
	RootComponent = EnemyMesh;
}

// initializes the enemy with the given parameters and activates it in the world
void ATowerEnemyBase::ActivateEnemy(FVector SpawnLocation, TArray<AActor*> AssignedPath, float RandomSpeed)
{
	SetActorLocation(SpawnLocation);
	PathToFollow = AssignedPath;
	MoveSpeed = RandomSpeed;
	CurrentHealth = Health;
	CurrentTargetIndex = 0;

	if (NormalMaterial) { EnemyMesh->SetMaterial(0, NormalMaterial); }

	bIsActive = true;
	SetActorHiddenInGame(false);
	SetActorTickEnabled(true);
	SetActorEnableCollision(true);
}

// deactivates the enemy and prepares it to be reused from the pool
void ATowerEnemyBase::DeactivateEnemy()
{
	bIsActive = false;
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	SetActorEnableCollision(false);
}

// called by towers when they attack the enemy, applying damage and checking for death
void ATowerEnemyBase::TakeDamage(float Damage)
{
	if (!bIsActive) return;

	CurrentHealth -= Damage;
	if (CurrentHealth <= 0)
	{
		// Talk to  Bank
		ATowerGameMode* GM = Cast<ATowerGameMode>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			GM->AddCoins(1);
		}

		// Broadcast the death and the exact location for the UI
		OnEnemyDied.Broadcast(this, GetActorLocation());

		// Send back to the pool
		DeactivateEnemy();
	}
}

// handles the enemy's movement along the assigned path and checks for reaching waypoints or the final destination
void ATowerEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsActive) return;

	if (PathToFollow.Num() > 0 && CurrentTargetIndex < PathToFollow.Num())
	{
		AActor* CurrentTarget = PathToFollow[CurrentTargetIndex];
		if (CurrentTarget)
		{
			FVector CurrentLoc = GetActorLocation();
			FVector TargetLoc = CurrentTarget->GetActorLocation();
			TargetLoc.Z = CurrentLoc.Z;

			FVector Direction = (TargetLoc - CurrentLoc).GetSafeNormal();
			SetActorLocation(CurrentLoc + (Direction * MoveSpeed * DeltaTime));

			if (FVector::Distance(CurrentLoc, TargetLoc) < 50.0f)
			{
				ATargetWaypoint* ReachedWaypoint = Cast<ATargetWaypoint>(CurrentTarget);
				if (ReachedWaypoint)
				{
					if (ReachedWaypoint->bIsFinalWaypoint)
					{
						TriggerBaseReached();
						return;
					}
					else if (ReachedWaypoint->bIsTeleportNode)
					{
						CurrentTargetIndex++;
						if (CurrentTargetIndex < PathToFollow.Num())
						{
							SetActorLocation(PathToFollow[CurrentTargetIndex]->GetActorLocation());
						}
					}
					else { CurrentTargetIndex++; }
				}
			}
		}
	}
}

// called when the enemy reaches the final waypoint, triggering the damage to the base and starting the flash effect
void ATowerEnemyBase::TriggerBaseReached()
{
	bIsActive = false;
	if (FlashMaterial) { EnemyMesh->SetMaterial(0, FlashMaterial); }
	GetWorld()->GetTimerManager().SetTimer(FlashTimerHandle, this, &ATowerEnemyBase::FinishBaseReached, 1.0f, false);
}

// finalizes the base reached event, broadcasting the damage to the base and resetting the enemy for reuse
void ATowerEnemyBase::FinishBaseReached()
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy Reached the Player! Dealt %f damage!"), DamageAmount);
	OnEnemyReachedBase.Broadcast(DamageAmount, this);
}