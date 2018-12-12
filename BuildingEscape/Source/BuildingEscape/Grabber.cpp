// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "CoreMinimal.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/EngineTypes.h"

#define OUT


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PhysicsHandle)
	{
		return;
	}
	//	if the physics handle is attached
	if (PhysicsHandle->GrabbedComponent)
	{
		//	move the object that we are holding to end of our line trace based on player orientation
		PhysicsHandle->SetTargetLocation(GetLineTraceEnd());
	}
}

void UGrabber::Grab()
{
	//UE_LOG(LogTemp, Warning, TEXT("Grab Pressed"));

	///	LINE TRACE and reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	if (!PhysicsHandle)
	{
		return;
	}
	///	If we hit something then attach a physics handle
	if (ActorHit)
	{
		// attach physics handle
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None,	//no bones needed
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true // constrainRotation
		);
	}
}

void UGrabber::Release()
{
	if (!PhysicsHandle)
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Grab Released"));
	//Release physics handle
	PhysicsHandle->ReleaseComponent();
}

///Look for attached Physics Handle
void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		//Physics handle is found
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s is missing PhysicsHandleComponent"), *GetOwner()->GetName());
	}
}

///Look for attached Input Component (only appears at run-time)
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);	//Bind the input grab axis
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);	//Bind the input release axis
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s is missing Input Component"), *GetOwner()->GetName());
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	///// Get player view point this tick
	//FVector PlayerViewPointLocation;
	//FRotator PlayerViewPointRotation;
	//GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation,
	//	OUT PlayerViewPointRotation);
	////UE_LOG(LogTemp, Warning, TEXT("Player Location: %s\nPlayer Rotation:%s\n"), *PlayerViewPointLocation.ToString(), *PlayerViewPointRotation.ToString());
	//
	///////Draw a red trace in the world
	//FVector LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);
	//DrawDebugLine(GetWorld(), 
	//	PlayerViewPointLocation, 
	//	LineTraceEnd, 
	//	FColor(255, 0, 0), false, 0.0f, 0.0f, 10.0f
	//);

	///Setup query Parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	/// Ray-cast out to reach distance
	FHitResult LineTraceHit;
	GetWorld()->LineTraceSingleByObjectType(OUT LineTraceHit,
		GetLineTraceStart(), 
		GetLineTraceEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	/////See what we hit
	////FString ObjectName = LineTraceHit.GetActor()->GetName();
	//AActor* ActorHit = LineTraceHit.GetActor();
	//if (ActorHit)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Actor Hit With Ray-cast: %s\n"), *ActorHit->GetName());
	//}
	
	//return FHitResult();

	return LineTraceHit;
}

const FVector UGrabber::GetLineTraceStart()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);
	//UE_LOG(LogTemp, Warning, TEXT("Player Location: %s\nPlayer Rotation:%s\n"), *PlayerViewPointLocation.ToString(), *PlayerViewPointRotation.ToString());

	///Use Player Location and Orientation to get line trace end
	FVector LineTraceStart = PlayerViewPointLocation;

	return LineTraceStart;
}

const FVector UGrabber::GetLineTraceEnd()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);
	//UE_LOG(LogTemp, Warning, TEXT("Player Location: %s\nPlayer Rotation:%s\n"), *PlayerViewPointLocation.ToString(), *PlayerViewPointRotation.ToString());

	///Use Player Location and Orientation to get line trace end
	FVector LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);

	return LineTraceEnd;
}




