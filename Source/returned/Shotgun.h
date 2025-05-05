// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponsBase.h"
#include "Shotgun.generated.h"

/**
 * 
 */
UCLASS()

class RETURNED_API AShotgun : public AWeaponsBase
{
	GENERATED_BODY()
	AShotgun();
public:
	void Attack() override;
	float Reload() override;
	
};
