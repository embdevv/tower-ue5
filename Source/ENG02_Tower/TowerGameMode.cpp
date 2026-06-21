#include "TowerGameMode.h"

void ATowerGameMode::AddCoins(int32 Amount)
{
	TotalCoins += Amount;
	UE_LOG(LogTemp, Warning, TEXT("Enemy killed! Bank updated: %d Coins"), TotalCoins);
}