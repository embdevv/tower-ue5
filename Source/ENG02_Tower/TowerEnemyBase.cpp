#include "TowerEnemyBase.h"
#include "TargetWaypoint.h" 
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "TowerGameMode.h" //includes the Bank system
#include "Components/SphereComponent.h"
#include "PenguinBase.h"
#include "Engine/DamageEvents.h"

// Sets default values
ATowerEnemyBase::ATowerEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;
	EnemyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnemyMesh"));
	RootComponent = EnemyMesh;

	//hitbox setup
	AttackCollision = CreateDefaultSubobject<USphereComponent>(TEXT("AttackCollision"));
	AttackCollision->SetupAttachment(RootComponent);
	AttackCollision->SetSphereRadius(60.0f); // zombie reach
	AttackCollision->OnComponentBeginOverlap.AddDynamic(this, &ATowerEnemyBase::OnAttackOverlapBegin);
}

// initializes the enemy with the given parameters and activates it in the world
void ATowerEnemyBase::ActivateEnemy(FVector SpawnLocation, TArray<AActor*> AssignedPath, float RandomSpeed)
{
	SetActorLocation(SpawnLocation);
	PathToFollow = AssignedPath;
	MoveSpeed = RandomSpeed;
	CurrentHealth = Health;
	CurrentTargetIndex = 0;

	// forget previous penguin target and reset the attack timer
	CurrentDefenderTarget = nullptr;
	AttackTimer = 0.0f;

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
	CurrentDefenderTarget = nullptr; // Clean up memory

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
		// Talk to the Bank
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

void ATowerEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsActive) return;

	// 
	// COMBAT STATE: when blocked by penguin

	// IsValid() returns false if penguin isnt there anymore aka died
	if (IsValid(CurrentDefenderTarget))
	{
		AttackTimer += DeltaTime;
		if (AttackTimer >= AttackCooldown)
		{
			AttackTimer = 0.0f; // Reset the swing timer

			UE_LOG(LogTemp, Warning, TEXT("Zombie throws a punch! Dealing %f damage to the penguin!"), DamageAmount);

			// attack penguin
			CurrentDefenderTarget->TakeDamage(DamageAmount, FDamageEvent(), nullptr, this);
		}

		// to stop enemy from moving while fighting. skip movement code below
		return;
	}
	else
	{
		// Clean up the memory pointer just in case the penguin died
		CurrentDefenderTarget = nullptr;
	}


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

// finalizes
void ATowerEnemyBase::FinishBaseReached()
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy Reached the Base! Dealt %f damage!"), DamageAmount);
	OnEnemyReachedBase.Broadcast(DamageAmount, this);
}

// COMBAT OVERLAP FUNCTION
void ATowerEnemyBase::OnAttackOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsActive) return;

	// walked into penguin checker
	APenguinBase* HitPenguin = Cast<APenguinBase>(OtherActor);

	// If penguin and not fighting somehting else, lock on to it and start the attack timer
	if (HitPenguin && !IsValid(CurrentDefenderTarget))
	{
		UE_LOG(LogTemp, Warning, TEXT("Zombie bumped into a Penguin! Initiating combat lock-on."));
		CurrentDefenderTarget = HitPenguin; // Lock on
		AttackTimer = AttackCooldown; // Instantly trigger the first punch
	}
}