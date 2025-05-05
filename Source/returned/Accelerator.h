// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Math/UnrealMathUtility.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Accelerator.generated.h"


UCLASS(ClassGroup= (Custom), meta=(BlueprintSpawnableComponent))
class RETURNED_API UAccelerator : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAccelerator();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

void MoveActor(bool ShouldActorMove);

private:

FVector FirstLocation;
UPROPERTY(EditAnywhere)
FVector Distance;
FVector TargetLocation;
float Speed;
UPROPERTY(EditAnywhere)
float MoveTime;
bool ShouldActorMove=false;
float RemainingTime;


void Accelerator(float DeltaTime);
};
