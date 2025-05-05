// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MainCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Turret.generated.h"



UCLASS()
class RETURNED_API ATurret : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATurret();
	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* Capsule;
    UPROPERTY(EditAnywhere)
    class UStaticMeshComponent* BaseMesh;
    UPROPERTY(EditAnywhere)
    class UStaticMeshComponent* BodyMesh;
	UPROPERTY(EditAnywhere)
    class UStaticMeshComponent* HeadMesh;
	UPROPERTY(EditAnywhere)
	class USceneComponent* SpawnP;
	void DestructionHandler();	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    void TargetDetected();
	bool IsLineBlocked(const FVector& Start, const FVector& End, AActor* IgnoreActor = nullptr);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
private:
	UPROPERTY(EditAnywhere, Category="Projectile")
	float FireRate = 0.2f; // Time in seconds between shots
	float LastFireTime;
	AMainCharacter* Character;
	FTimerHandle Zamanlayici;
	float SecondsCounter;
	float StartTime;
	UPROPERTY(EditAnywhere, Category="Projectile")
	TSubclassOf<class AProjectile> ProjectileClass;


};
