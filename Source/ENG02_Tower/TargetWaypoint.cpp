#include "TargetWaypoint.h"


// Sets default values
ATargetWaypoint::ATargetWaypoint()
{
	PrimaryActorTick.bCanEverTick = false;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
}