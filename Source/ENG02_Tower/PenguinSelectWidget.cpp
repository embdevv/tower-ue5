// PenguinSelectWidget.cpp
#include "PenguinSelectWidget.h"
#include "PenguinSpawnPoint.h"
#include "PenguinSpawnSubsystem.h"

void UPenguinSelectWidget::Setup(APenguinSpawnPoint* InSpawnPoint)
{
    TargetSpawnPoint = InSpawnPoint;
}

void UPenguinSelectWidget::TrySpawn(TSubclassOf<APenguinBase> PenguinClass)
{
    if (!TargetSpawnPoint) return;

    UPenguinSpawnSubsystem* Subsystem = GetWorld()->GetSubsystem<UPenguinSpawnSubsystem>();
    bool bSuccess = Subsystem->TrySpawnPenguin(PenguinClass, TargetSpawnPoint);

    if (bSuccess)
    {
        RemoveFromParent();
    }
}

void UPenguinSelectWidget::ChooseDefensive() { TrySpawn(DefensiveClass); }
void UPenguinSelectWidget::ChooseShortRange() { TrySpawn(ShortRangeClass); }
void UPenguinSelectWidget::ChooseLongRange() { TrySpawn(LongRangeClass); }