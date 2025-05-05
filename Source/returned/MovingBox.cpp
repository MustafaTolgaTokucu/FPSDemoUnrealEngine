// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingBox.h"

// Sets default values
AMovingBox::AMovingBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMovingBox::BeginPlay()
{
	Super::BeginPlay();
	FirstLocation= GetActorLocation();
	
}

// Called every frame
void AMovingBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
   
    MovingPlatform(DeltaTime);
    RotatingPlatform(DeltaTime);
    
}


void AMovingBox::MovingPlatform(float DeltaTime)
{

if(ShouldPlatformReturn(DeltaTime))
{	
    UnitMoved = ReturnPoint* PlatformVelocity.GetUnsafeNormal();
	ExactPosition=FirstLocation + UnitMoved;
	
	SetActorLocation(ExactPosition);
    PlatformVelocity= -PlatformVelocity;
	FirstLocation=ExactPosition;
}
else{
FVector CurrentPosition = GetActorLocation();
    NextPosition = CurrentPosition + PlatformVelocity*DeltaTime;
	SetActorLocation(NextPosition);
	Distance = FVector::Dist(NextPosition , FirstLocation);
}

}
bool AMovingBox::ShouldPlatformReturn(float DeltaTime) 
{
	FVector CurrentPosition = GetActorLocation();
	NextPosition = CurrentPosition + PlatformVelocity*DeltaTime;
	SetActorLocation(NextPosition);
	Distance = FVector::Dist(NextPosition , FirstLocation);
return Distance>ReturnPoint;
}
void AMovingBox::RotatingPlatform(float DeltaTime)
{
AddActorLocalRotation(RotationVelocity*DeltaTime);

}