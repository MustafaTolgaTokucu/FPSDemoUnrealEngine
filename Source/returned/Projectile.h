// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Projectile.generated.h"

UCLASS()
class RETURNED_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:
    UPROPERTY(EditAnywhere)
	float DamageAmount=10.f;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MermiMesh;
	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* PMovement;
    UFUNCTION()
	void OnHit(UPrimitiveComponent* hitter, AActor* hitActor, UPrimitiveComponent* OtherObject, FVector ImpulseHit, const FHitResult& HitResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
