// PenguinBase.cpp
#include "PenguinBase.h"
#include "TowerEnemyBase.h"
#include "Components/SphereComponent.h"
#include "TimerManager.h"
#include "Engine/DamageEvents.h"

APenguinBase::APenguinBase()
{
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;

    // Create the invisible detection sphere for all penguins
    RangeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("RangeSphere"));
    RangeSphere->SetupAttachment(RootComponent);
    RangeSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}

void APenguinBase::BeginPlay()
{
    Super::BeginPlay();
    CurrentHealth = MaxHealth;

    // Automatically size the sphere to match whatever number RV typed in the bp
    RangeSphere->SetSphereRadius(AttackRange);

    // If this penguin is not the defense type, turn on the attack timer
    if (AttackSpeed > 0.f && AttackDamage > 0.f)
    {
        GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &APenguinBase::PerformAttackCheck, AttackSpeed, true);
    }
}

float APenguinBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    CurrentHealth -= ActualDamage;
    OnDamaged(ActualDamage);

    if (CurrentHealth <= 0.f)
    {
        OnDeath();
        Destroy();
    }

    return ActualDamage;
}

// checks for enemies in range and attacks first valid enemy found
// If the penguin has a projectile, it will fire the projectile instead of applying damage directly. gives delay for burst fire
void APenguinBase::PerformAttackCheck()
{
    TArray<AActor*> OverlappingActors;
    RangeSphere->GetOverlappingActors(OverlappingActors, ATowerEnemyBase::StaticClass());

    for (AActor* Actor : OverlappingActors)
    {
        ATowerEnemyBase* Enemy = Cast<ATowerEnemyBase>(Actor);
        if (Enemy && Enemy->bIsActive)
        {

            UE_LOG(LogTemp, Warning, TEXT("%s is attacking %s for %f damage!"),
                *GetName(), *Enemy->GetName(), AttackDamage);

            if (ProjectileClass)
            {
                CurrentTarget = Enemy;
                FireBurst();
                GetWorld()->GetTimerManager().SetTimer(BurstTimerHandle, this, &APenguinBase::FireBurst, 0.2f, false);
            }
            else
            {
                Enemy->ReceiveDamage(AttackDamage);
            }
            break;
        }
    }
}

void APenguinBase::FireBurst()
{
    // Double check that the target didn't die in the 0.2 second delay window
    if (ProjectileClass && IsValid(CurrentTarget) && CurrentTarget->bIsActive)
    {

        UE_LOG(LogTemp, Warning, TEXT("%s fires projectile at %s!"), *GetName(), *CurrentTarget->GetName());
        // Spawn the arrow!
        GetWorld()->SpawnActor<AActor>(ProjectileClass, GetActorLocation(), GetActorRotation());
    }
}