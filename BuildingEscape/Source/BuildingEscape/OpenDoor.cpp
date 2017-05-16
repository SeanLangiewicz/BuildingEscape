// Sean Langiewicz2017

#include "BuildingEscape.h"
#include "OpenDoor.h"
#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();

	if (!PressurePlate)
	{
		//UE_LOG(LogTemp,Error,TEXT("%s missing pressure plate"),*GetOwner()->GetName())
	}
   
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	//Poll the trigger volume every frame
	if (GetTotalMassofActorsOnPlate() > TriggerMass) //TODO make into a parameters
	{
		 //if the ActorThatOpens is in the volume
		OnOpen.Broadcast();		
	}
	else
	{
		OnClose.Broadcast();
	}
	
}
float UOpenDoor:: GetTotalMassofActorsOnPlate()
{
	float TotalMass = 0.f;

	//find all the overlapping actors
	TArray <AActor*> OverLappingActors;
	if (!PressurePlate)
	{
		return TotalMass;
	}
	PressurePlate->GetOverlappingActors(OUT OverLappingActors);

	//Iterate through them adding their masses
	for (const auto* Actor : OverLappingActors)
	{
	TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate"), *Actor->GetName());
	}

	return TotalMass;
}

