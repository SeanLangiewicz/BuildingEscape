// Sean Langiewicz2017

#include "BuildingEscape.h"
#include "Grabber.h"
#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
	GetFirstPhysicsBodyInReach();

}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PhysicsHandle)
	{
		return;
	}

	// if the physics is handle is attached
	if (PhysicsHandle->GrabbedComponent)
	{
		//move object holding
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}


}

void UGrabber::FindPhysicsHandleComponent()
{
	
	///Look for attached physics handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		//Message that physic handle is missing.
		UE_LOG(LogTemp, Error, TEXT("%s missing physic handle component"), *GetOwner()->GetName());
	}

}

//Look for attached input component (only appears at run time)
void UGrabber::SetupInputComponent()
{

	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		///Bind the Input Axis
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		//Message that physic handle is missing.
		UE_LOG(LogTemp, Error, TEXT("%s missing Input component"), *GetOwner()->GetName());

	}
}




void UGrabber::Grab()
{

	///LINE Trace and release any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();//Gets tthe mesh in our case
	auto ActorHit = HitResult.GetActor();

	///If we hit something then attach a physics handle
	if (ActorHit)
	{
		if (!PhysicsHandle)
		{
			return;
		}
		
		
		PhysicsHandle->GrabComponent(ComponentToGrab, 
			NAME_None, //No Bones needed
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true); //Allow rotation
	}
	
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Release Object"))

		if (!PhysicsHandle)
		{
			return;
		}
		PhysicsHandle->ReleaseComponent();
}



const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	///Setup Query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByObjectType(OUT HitResult, GetReachLineStart(), GetReachLineEnd(), FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParameters);

	
	return HitResult;
}

FVector UGrabber:: GetReachLineEnd()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, PlayerViewPointRotation);

	return PlayerViewPointLocation + PlayerViewPointRotation.Vector()*Reach;
}

FVector UGrabber::GetReachLineStart()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, PlayerViewPointRotation);

	return PlayerViewPointLocation;
}