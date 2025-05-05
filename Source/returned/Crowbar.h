// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponsBase.h"
#include "Crowbar.generated.h"

/**
 * 
 */
UCLASS()
class RETURNED_API ACrowbar : public AWeaponsBase
{
	GENERATED_BODY()
	ACrowbar();
	public:
	void Attack() override;
	
};
