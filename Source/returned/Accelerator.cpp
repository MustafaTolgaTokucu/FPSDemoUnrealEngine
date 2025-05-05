// Fill out your copyright notice in the Description page of Project Settings.


#include "Accelerator.h"

// Sets default values for this component's properties
UAccelerator::UAccelerator()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
    
	MoveActor(false);
	// ...
}


// Called when the game starts
void UAccelerator::BeginPlay()
{
	Super::BeginPlay();
   FirstLocation = GetOwner()->GetActorLocation();
}


// Called every frame
void UAccelerator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
	Accelerator(DeltaTime);
}

void UAccelerator::MoveActor(bool TriggerSaidMove)
{
    ShouldActorMove=TriggerSaidMove;
	
}

void UAccelerator::Accelerator(float DeltaTime)
{
	//UE_LOG(LogTemp, Warning, TEXT("%s"),(ShouldActorMove ? TEXT("true"): TEXT("false")));
	    if(ShouldActorMove){
    FVector CurrentLocation = GetOwner()->GetActorLocation();
	TargetLocation = FirstLocation + Distance;
	Speed = FVector::Distance(FirstLocation,TargetLocation)/MoveTime;
	FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation,DeltaTime,Speed);
	GetOwner()->SetActorLocation(NewLocation);
	//RemainingTime=(FVector::Distance(CurrentLocation,TargetLocation)/Speed);
	//UE_LOG(LogTemp, Display, TEXT("Remaining Time: %f"), RemainingTime);
	}
	else
{
	FVector CurrentLocation = GetOwner()->GetActorLocation();
	FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, FirstLocation,DeltaTime,Speed);
	GetOwner()->SetActorLocation(NewLocation);
}
    
}