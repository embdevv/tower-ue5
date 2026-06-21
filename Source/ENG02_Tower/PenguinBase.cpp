// PenguinBase.cpp
#include "PenguinBase.h"

APenguinBase::APenguinBase()
{
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;

    bCanBeDamaged = true; // required — gates whether TakeDamage does anything
}

void APenguinBase::BeginPlay()
{
    Super::BeginPlay();
    CurrentHealth = MaxHealth;
}

float APenguinBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
    AController* EventInstigator, AActor* DamageCauser)
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