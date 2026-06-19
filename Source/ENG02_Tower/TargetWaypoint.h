#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TargetWaypoint.generated.h"

UCLASS()
class ENG02_TOWER_API ATargetWaypoint : public AActor
{
	GENERATED_BODY()
public:
	ATargetWaypoint();

	// visual component so you can see it in the editor
	UPROPERTY(VisibleAnywhere, Category = "Waypoint Settings")
	class USceneComponent* Root;

	// Check this box in the Editor if this waypoint is an EXIT door
	UPROPERTY(EditAnywhere, Category = "Waypoint Settings")
	bool bIsTeleportNode = false;
};