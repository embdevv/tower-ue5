#include "TargetWaypoint.h"

ATargetWaypoint::ATargetWaypoint()
{
	PrimaryActorTick.bCanEverTick = false;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
}