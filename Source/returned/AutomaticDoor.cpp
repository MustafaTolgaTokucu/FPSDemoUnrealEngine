// Fill out your copyright notice in the Description page of Project Settings.


#include "AutomaticDoor.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AAutomaticDoor::AAutomaticDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    DoorMover=CreateDefaultSubobject<UAccelerator>(TEXT("DoorMoverComponent"));
	DoorTrigger=CreateDefaultSubobject<UTriggerBoxComponente>(TEXT("TriggerBoxForDoor"));
	Mesh=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	RootComponent=Mesh;
	
}

// Called when the game starts or when spawned
void AAutomaticDoor::BeginPlay()
{
	Super::BeginPlay();
	DoorTrigger->SetMover(DoorMover);
}

// Called every frame
void AAutomaticDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

