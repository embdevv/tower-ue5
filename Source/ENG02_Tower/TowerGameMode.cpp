#include "TowerGameMode.h"

void ATowerGameMode::AddCoins(int32 Amount)
{
	TotalCoins += Amount;
	UE_LOG(LogTemp, Warning, TEXT("Enemy killed! Bank updated: %d Coins"), TotalCoins);
}

void ATowerGameMode::TakePlayerDamage(float Damage)
{
	PlayerHealth -= Damage;

	// Print to the debug log
	UE_LOG(LogTemp, Error, TEXT("PLAYER TOOK DAMAGE! Remaining Health: %f"), PlayerHealth);

	if (PlayerHealth <= 0.0f)
	{
		UE_LOG(LogTemp, Error, TEXT("=== GAME OVER: THE BEAR HAS FALLEN ==="));
	}
}