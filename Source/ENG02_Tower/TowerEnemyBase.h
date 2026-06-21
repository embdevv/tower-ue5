// TowerEnemyBase.h
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TowerEnemyBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEnemyReachedBaseSignature, float, DamageAmount, ATowerEnemyBase*, Enemy);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEnemyDiedSignature, ATowerEnemyBase*, Enemy, FVector, DeathLocation);

// Base class for all enemies in the tower defense game, handling movement, health, and interactions with towers and the base
UCLASS()
class ENG02_TOWER_API ATowerEnemyBase : public AActor
{
	GENERATED_BODY()
public:
	ATowerEnemyBase();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Visuals")
	class UStaticMeshComponent* EnemyMesh;

	UPROPERTY(EditAnywhere, Category = "Enemy Visuals")
	class UMaterialInterface* NormalMaterial;

	UPROPERTY(EditAnywhere, Category = "Enemy Visuals")
	class UMaterialInterface* FlashMaterial;

	UPROPERTY(EditAnywhere, Category = "Enemy Stats")
	float Health = 10.0f;

	float CurrentHealth;

	UPROPERTY(EditAnywhere, Category = "Enemy Stats")
	float MoveSpeed = 150.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Stats")
	float DamageAmount = 1.0f;

	TArray<AActor*> PathToFollow;
	int32 CurrentTargetIndex = 0;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnEnemyReachedBaseSignature OnEnemyReachedBase;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnEnemyDiedSignature OnEnemyDied;

	bool bIsActive = false;
	void ActivateEnemy(FVector SpawnLocation, TArray<AActor*> AssignedPath, float RandomSpeed);
	void DeactivateEnemy();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ReceiveDamage(float Damage);

	FTimerHandle FlashTimerHandle;
	void TriggerBaseReached();
	void FinishBaseReached();

	// The Hitbox to detect penguins
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	class USphereComponent* AttackCollision;

	// The cooldown time between attacks
	UPROPERTY(EditAnywhere, Category = "Enemy Stats")
	float AttackCooldown = 2.5f;

	float AttackTimer = 0.0f;

	// The penguin currently blocking our path
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	class APenguinBase* CurrentDefenderTarget;

	UFUNCTION()
	void OnAttackOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};