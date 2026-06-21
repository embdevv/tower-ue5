// PenguinSelectWidget.h
#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PenguinBase.h"
#include "PenguinSelectWidget.generated.h"

class APenguinSpawnPoint;
class APenguinBase;

UCLASS()
class UPenguinSelectWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable)
    void Setup(APenguinSpawnPoint* InSpawnPoint);

    UFUNCTION(BlueprintCallable)
    void ChooseDefensive();

    UFUNCTION(BlueprintCallable)
    void ChooseShortRange();

    UFUNCTION(BlueprintCallable)
    void ChooseLongRange();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<APenguinBase> DefensiveClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<APenguinBase> ShortRangeClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<APenguinBase> LongRangeClass;

protected:
    UPROPERTY(BlueprintReadOnly)
    APenguinSpawnPoint* TargetSpawnPoint;

private:
    void TrySpawn(TSubclassOf<APenguinBase> PenguinClass);
};