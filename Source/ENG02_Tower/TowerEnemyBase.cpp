#include "TowerEnemyBase.h"
#include "TargetWaypoint.h" 
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"

ATowerEnemyBase::ATowerEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;
	EnemyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnemyMesh"));
	RootComponent = EnemyMesh;
}

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

void ATowerEnemyBase::DeactivateEnemy()
{
	bIsActive = false;
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	SetActorEnableCollision(false);
}

void ATowerEnemyBase::TakeDamage(float Damage)
{
	if (!bIsActive) return;

	CurrentHealth -= Damage;
	if (CurrentHealth <= 0)
	{
		OnEnemyDied.Broadcast(this);
	}
}

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

void ATowerEnemyBase::TriggerBaseReached()
{
	bIsActive = false;

	if (FlashMaterial) { EnemyMesh->SetMaterial(0, FlashMaterial); }

	GetWorld()->GetTimerManager().SetTimer(FlashTimerHandle, this, &ATowerEnemyBase::FinishBaseReached, 1.0f, false);
}

void ATowerEnemyBase::FinishBaseReached()
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy Reached the Scientist! Dealt %f damage!"), DamageAmount);
	OnEnemyReachedBase.Broadcast(DamageAmount, this);
}