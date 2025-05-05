// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingBox.generated.h"

UCLASS()
class RETURNED_API AMovingBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovingBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
private:	
	UPROPERTY(EditAnywhere)
    FVector NextPosition;
	UPROPERTY(EditAnywhere, Category="Platform Properties")

    FVector PlatformVelocity;
    UPROPERTY(EditAnywhere)
    float Distance;
	FVector FirstLocation;
    UPROPERTY(EditAnywhere, Category="Platform Properties")
	float ReturnPoint;
	UPROPERTY(EditAnywhere)
	FVector ExactPosition;
	UPROPERTY(EditAnywhere)
	FVector UnitMoved;
    UPROPERTY(EditAnywhere, Category="Platform Properties")
	FRotator RotationVelocity;
void MovingPlatform(float DeltaTime);
void RotatingPlatform(float DeltaTime);
bool ShouldPlatformReturn(float Deltatime);

};
