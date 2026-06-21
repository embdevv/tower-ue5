#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TowerGameMode.generated.h"

UCLASS()
class ENG02_TOWER_API ATowerGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	// The central bank variable
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Economy")
	int32 TotalCoins = 0;

	// function enemies will call when they die
	UFUNCTION(BlueprintCallable, Category = "Economy")
	void AddCoins(int32 Amount);

	// The Player Health variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float PlayerHealth = 100.0f;

	UFUNCTION(BlueprintCallable, Category = "Player Stats")
	void TakePlayerDamage(float Damage);


};