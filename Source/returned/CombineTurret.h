// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "CombineTurret.generated.h"

UCLASS()
class RETURNED_API ACombineTurret : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACombineTurret();
	void DestructionHandler();
	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* BodyMesh;
	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* HeadMesh;
	UPROPERTY(EditAnywhere)
	class USceneComponent* BodyPivot;
	UPROPERTY(EditAnywhere)
	class USceneComponent* HeadPivot;
	UPROPERTY(EditAnywhere)
	class USceneComponent* SpawnP;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frameF
	virtual void Tick(float DeltaTime) override;


private:
	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;
	
	
};
