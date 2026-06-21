// MyPlayerController.h
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

class UPenguinSelectWidget;
class APenguinSpawnPoint;
class UInputMappingContext;
class UInputAction;

UCLASS()
class AMyPlayerController : public APlayerController
{
    GENERATED_BODY()
public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UPenguinSelectWidget> PenguinSelectWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputMappingContext* DefaultMappingContext;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* ClickAction;

protected:
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;

    void OnLeftClick();

private:
    UPenguinSelectWidget* ActiveWidget = nullptr;
};