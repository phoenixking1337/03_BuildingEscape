// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Components/PrimitiveComponent.h"
#include "Grabber.h"

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

	Owner = GetOwner(); //Find the owning Actor

	//OpenDoor();
	//ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the Trigger Volume
	// If the ActorThatOpens is in the volume
	//Open Door
	//if (PressurePlate && PressurePlate->IsOverlappingActor(ActorThatOpens))
	if(GetTotalMassOfActorsOnPlate() > 40.0f) //TODO make into a parameter
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	else if ((GetWorld()->GetTimeSeconds() - LastDoorOpenTime) > DoorCloseDelay) //Check if it's time to close the door
	{
		CloseDoor();
	}
}

void UOpenDoor::OpenDoor()
{
	//AActor* Owner = GetOwner(); //Find the owning Actor

	//FRotator NewRotation = FRotator(0.0f, -60.0f, 0.0f); //Create a reactor

	Owner->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f)); //Set the door rotation
}

void UOpenDoor::CloseDoor()
{
	//AActor* Owner = GetOwner(); //Find the owning Actor

	//FRotator NewRotation = FRotator(0.0f, -60.0f, 0.0f); //Create a reactor

	Owner->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f)); //Set the door rotation
}

///returns total mass in kg
float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float totalMass = 0.0f;

	///find all the overlapping actors
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	///iterate through them adding their masses
	if (OverlappingActors.Num() != 0)
	{	
		for (const auto* actorItem : OverlappingActors)
		{
			FString ObjectName = actorItem->GetName();
			UE_LOG(LogTemp, Warning, TEXT("%s is is in Zone"), *ObjectName);
			totalMass += actorItem->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		}
	}
	return totalMass;
}

