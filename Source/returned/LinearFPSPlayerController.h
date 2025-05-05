// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LinearFPSPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class RETURNED_API ALinearFPSPlayerController : public APlayerController
{
	GENERATED_BODY()
	

public:
    void SetPlayerEnabledState(bool bPlayerEnabled);

};
