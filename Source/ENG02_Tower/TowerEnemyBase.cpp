#include "TowerEnemyBase.h"
#include "TargetWaypoint.h" 
#include "TimerManager.h"

// Sets default values
ATowerEnemyBase::ATowerEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;
	EnemyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnemyMesh"));
	RootComponent = EnemyMesh;
}

// Activates the enemy with the given spawn location, path to follow, and movement speed. 
// Resets health and other necessary variables for the enemy to function properly when reused from the pool
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
// Deactivates the enemy, hiding it and disabling tick and collision until it's reused from the pool
void ATowerEnemyBase::DeactivateEnemy()
{
	bIsActive = false;
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	SetActorEnableCollision(false);
}

// Called when the enemy takes damage, either from a tower or other source
void ATowerEnemyBase::TakeDamage(float Damage)
{
	if (!bIsActive) return;

	CurrentHealth -= Damage;
	if (CurrentHealth <= 0)
	{
		OnEnemyDied.Broadcast(this);
	}
}

// Called every frame to move the enemy along its assigned path towards the base, and check for reaching waypoints or the final destination
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

			// stopper so no overshooting if enemy is fast
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
					else
					{
						CurrentTargetIndex++;
					}
				}
			}
		}
	}
}

// Called when the enemy reaches the base (final waypoint)
void ATowerEnemyBase::TriggerBaseReached()
{
	bIsActive = false;
	if (FlashMaterial) { EnemyMesh->SetMaterial(0, FlashMaterial); }

	GetWorld()->GetTimerManager().SetTimer(FlashTimerHandle, this, &ATowerEnemyBase::FinishBaseReached, 1.0f, false);
}

// Finishes the base reached sequence, broadcasting the event to apply damage to the player and then allowing the enemy to be deactivated and returned to the pool
void ATowerEnemyBase::FinishBaseReached()
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy Reached the Scientist! Dealt %f damage!"), DamageAmount);
	OnEnemyReachedBase.Broadcast(DamageAmount, this);
}