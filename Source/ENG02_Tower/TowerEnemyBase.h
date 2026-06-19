#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TowerEnemyBase.generated.h"

UCLASS()
class ENG02_TOWER_API ATowerEnemyBase : public AActor
{
	GENERATED_BODY()
public:
	ATowerEnemyBase();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Enemy Visuals")
	class UStaticMeshComponent* EnemyMesh;

	UPROPERTY(EditAnywhere, Category = "Enemy Stats")
	float Health = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Stats")
	float MoveSpeed = 150.0f;

	UPROPERTY(VisibleAnywhere, Category = "Pathfinding")
	TArray<AActor*> PathToFollow;

	int32 CurrentTargetIndex = 0;
};