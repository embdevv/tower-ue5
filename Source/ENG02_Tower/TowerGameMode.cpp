#include "TowerGameMode.h"

void ATowerGameMode::BeginPlay()
{
    Super::BeginPlay();
    TimeRemaining = WaveDuration;
    GetWorld()->GetTimerManager().SetTimer(
        GameTimerHandle, this, &ATowerGameMode::TickGameTimer, 1.f, true
    );
}

void ATowerGameMode::TickGameTimer()
{
    if (bGameOver) return;
    TimeRemaining -= 1.f;
    OnTimerTick.Broadcast(TimeRemaining);
    if (TimeRemaining <= 0.f) TriggerVictory();
}

void ATowerGameMode::AddCoins(int32 Amount)
{
    TotalCoins += Amount;
    OnCoinsChanged.Broadcast(TotalCoins);
}

bool ATowerGameMode::SpendCoins(int32 Amount)
{
    if (TotalCoins < Amount) return false;
    TotalCoins -= Amount;
    OnCoinsChanged.Broadcast(TotalCoins);
    return true;
}

void ATowerGameMode::TakePlayerDamage(float Damage)
{
    if (bGameOver) return;
    PlayerHealth -= Damage;
    if (PlayerHealth <= 0.f) TriggerGameOver();
}

void ATowerGameMode::TriggerVictory()
{
    bGameOver = true;
    GetWorld()->GetTimerManager().ClearTimer(GameTimerHandle);
    OnVictory.Broadcast();
}

void ATowerGameMode::TriggerGameOver()
{
    bGameOver = true;
    GetWorld()->GetTimerManager().ClearTimer(GameTimerHandle);
    OnGameOver.Broadcast();
}