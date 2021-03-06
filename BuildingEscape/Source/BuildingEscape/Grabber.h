// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/InputComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	
	float Reach = 100.0f; //Player grabbing reach

	UPhysicsHandleComponent* PhysicsHandle = nullptr;

	UInputComponent* InputComponent = nullptr;

	//Ray-cast and grab what is in reach
	void Grab();

	//Grab released
	void Release();

	//Find (assumed) attached Physics Handle
	void FindPhysicsHandleComponent();

	//Setup (assumed) input component
	void SetupInputComponent();

	//Return hit for first physics body in reach
	const FHitResult GetFirstPhysicsBodyInReach();

	//Return player view point Line Trace Start
	const FVector GetLineTraceStart();

	//Return player view point Line Trace End
	const FVector GetLineTraceEnd();

};
