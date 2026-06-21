// TowerGameMode.h - replace your current header with this full version
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TowerGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameOver);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnVictory);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCoinsChanged, int32, NewAmount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimerTick, float, TimeRemaining);

UCLASS()
class ENG02_TOWER_API ATowerGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Economy")
    int32 TotalCoins = 50;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
    float WaveDuration = 120.f;

    UPROPERTY(BlueprintReadOnly, Category = "Timer")
    float TimeRemaining = 0.f;

    UPROPERTY(BlueprintReadOnly, Category = "Player Stats")
    float PlayerHealth = 100.f;

    UPROPERTY(BlueprintReadOnly, Category = "Game State")
    bool bGameOver = false;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnGameOver OnGameOver;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnVictory OnVictory;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnCoinsChanged OnCoinsChanged;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnTimerTick OnTimerTick;

    UFUNCTION(BlueprintCallable, Category = "Economy")
    void AddCoins(int32 Amount);

    UFUNCTION(BlueprintCallable, Category = "Economy")
    bool SpendCoins(int32 Amount);

    UFUNCTION(BlueprintCallable, Category = "Player Stats")
    void TakePlayerDamage(float Damage);

protected:
    virtual void BeginPlay() override;

private:
    FTimerHandle GameTimerHandle;
    void TickGameTimer();
    void TriggerVictory();
    void TriggerGameOver();
};