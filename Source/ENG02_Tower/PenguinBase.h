// PenguinBase.h
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PenguinBase.generated.h"

UCLASS(Blueprintable)
class ENG02_TOWER_API APenguinBase : public APawn
{
    GENERATED_BODY()
public:
    APenguinBase();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UStaticMeshComponent* Mesh;

    // Universal Hitbox for all penguins
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class USphereComponent* RangeSphere;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Penguin")
    int32 CoinCost = 1;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Penguin")
    float MaxHealth = 100.f;

    UPROPERTY(BlueprintReadOnly, Category = "Penguin")
    float CurrentHealth;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Penguin|Attack")
    float AttackRange = 0.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Penguin|Attack")
    float AttackDamage = 0.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Penguin|Attack")
    float AttackSpeed = 1.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Penguin|Attack")
    TSubclassOf<AActor> ProjectileClass;

    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

    UFUNCTION(BlueprintImplementableEvent)
    void OnDamaged(float DamageAmount);

    UFUNCTION(BlueprintImplementableEvent)
    void OnDeath();

    UFUNCTION(BlueprintImplementableEvent)
    void OnAttack(AActor* Target);

    // Combat Logic
    FTimerHandle AttackTimerHandle;
    FTimerHandle BurstTimerHandle;

    UPROPERTY()
    class ATowerEnemyBase* CurrentTarget;

    void PerformAttackCheck();

    UFUNCTION()
    void FireBurst();

protected:
    virtual void BeginPlay() override;
};