// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Hareketlendirici.h"
#include "TriggerBoxComponente.h"
#include "AutomaticDoor.generated.h"
class UStaticMeshComponent;
UCLASS()
class RETURNED_API AAutomaticDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAutomaticDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere)
    UAccelerator* DoorMover;
	UPROPERTY(EditAnywhere)
	UTriggerBoxComponente* DoorTrigger;
	UPROPERTY(EditAnywhere)
    UStaticMeshComponent* Mesh;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
    
};
