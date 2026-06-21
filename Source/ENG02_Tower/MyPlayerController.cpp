#include "MyPlayerController.h"
#include "PenguinSelectWidget.h"
#include "PenguinSpawnPoint.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void AMyPlayerController::BeginPlay()
{
    Super::BeginPlay();
    bShowMouseCursor = true;
    SetInputMode(FInputModeGameAndUI());

    if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
        ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        if (DefaultMappingContext)
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }
}

void AMyPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
    {
        if (ClickAction)
        {
            EnhancedInput->BindAction(ClickAction, ETriggerEvent::Started, this, &AMyPlayerController::OnLeftClick);
        }
    }
}

void AMyPlayerController::OnLeftClick()
{
    FHitResult Hit;
    if (GetHitResultUnderCursor(ECC_Visibility, false, Hit))
    {
        if (APenguinSpawnPoint* SpawnPoint = Cast<APenguinSpawnPoint>(Hit.GetActor()))
        {
            if (!SpawnPoint->IsOccupied() && PenguinSelectWidgetClass)
            {
                if (ActiveWidget) ActiveWidget->RemoveFromParent();

                ActiveWidget = CreateWidget<UPenguinSelectWidget>(this, PenguinSelectWidgetClass);
                ActiveWidget->Setup(SpawnPoint);
                ActiveWidget->AddToViewport();
            }
        }
    }
}