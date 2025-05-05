// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponsBase.h"
#include "Glock.generated.h"

/**
 * 
 */
UCLASS()
class RETURNED_API AGlock : public AWeaponsBase
{
	GENERATED_BODY()
	AGlock();

public:


	void Attack() override;
	void CombineAttack() override;
	float Reload() override;
	

protected:
	//virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere)
	class UParticleSystem* MuzzleFlash;
	UPROPERTY(EditAnywhere)
	class UParticleSystem* ImpactEffect;
	
private:
	
};
