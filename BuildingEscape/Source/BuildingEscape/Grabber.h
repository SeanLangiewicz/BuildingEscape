// Sean Langiewicz2017

#pragma once

#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();
	//How Far ahead of the player can we reach in cm
	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private :
	float Reach = 100.0f;

	UPhysicsHandleComponent*PhysicsHandle = nullptr;
	UInputComponent*InputComponent = nullptr;

	//Ray-cast and grab whats in reach.
	void Grab();
	//Release pressed
	void Release();

	// Find assumed attached physics handle
	void FindPhysicsHandleComponent();
	// Setup (Assumed) attached input component
	void SetupInputComponent();

	//Return hit for first phsyics body in reach
	const FHitResult GetFirstPhysicsBodyInReach();
	//Returns current end of reach line
	FVector GetReachLineEnd();

	//Returns start reach line
	FVector GetReachLineStart();
};
