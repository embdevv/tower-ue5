#include "TowerEnemyBase.h"
#include "TargetWaypoint.h" 

ATowerEnemyBase::ATowerEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;
	EnemyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnemyMesh"));
	RootComponent = EnemyMesh;
}

void ATowerEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If we have a path and haven't reached the end
	if (PathToFollow.Num() > 0 && CurrentTargetIndex < PathToFollow.Num())
	{
		AActor* CurrentTarget = PathToFollow[CurrentTargetIndex];
		if (CurrentTarget)
		{
			FVector CurrentLoc = GetActorLocation();
			FVector TargetLoc = CurrentTarget->GetActorLocation();

			// Ignore height differences while walking so it doesn't float up
			TargetLoc.Z = CurrentLoc.Z;

			// Move towards target
			FVector Direction = (TargetLoc - CurrentLoc).GetSafeNormal();
			SetActorLocation(CurrentLoc + (Direction * MoveSpeed * DeltaTime));

			// Check if we are close enough to the waypoint (aka reached)
			if (FVector::Distance(CurrentLoc, TargetLoc) < 25.0f)
			{
				// Cast to check if this specific waypoint is a teleport door
				ATargetWaypoint* ReachedWaypoint = Cast<ATargetWaypoint>(CurrentTarget);

				if (ReachedWaypoint && ReachedWaypoint->bIsTeleportNode)
				{
					// It IS a teleport door. Increment index to the next floor's door
					CurrentTargetIndex++;

					// Instantly teleport the enemy to the next floor's door
					if (CurrentTargetIndex < PathToFollow.Num())
					{
						SetActorLocation(PathToFollow[CurrentTargetIndex]->GetActorLocation());
					}
				}
				else
				{
					// Normal waypoint, just walk to the next one
					CurrentTargetIndex++;
				}
			}
		}
	}
	else if (CurrentTargetIndex >= PathToFollow.Num() && PathToFollow.Num() > 0)
	{
		// Reached the very end of the path 
		Destroy();
	}
}