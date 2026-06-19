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

	UPROPERTY(VisibleAnywhere, Category = "Waypoint Settings")
	class USceneComponent* Root;

	// Teleports the enemy to the next node
	UPROPERTY(EditAnywhere, Category = "Waypoint Settings")
	bool bIsTeleportNode = false;

	// Check this ONLY for the very last waypoint aka lose condition (we die or hella damage)
	UPROPERTY(EditAnywhere, Category = "Waypoint Settings")
	bool bIsFinalWaypoint = false;
};