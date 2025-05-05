// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LinearFPSGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class RETURNED_API ALinearFPSGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
    
    void ActorDied(AActor* DeadActor, AActor* Killer);
    void AtImpact(AActor* ImpactActor);
protected:
    virtual void BeginPlay() override;

    UFUNCTION(BlueprintImplementableEvent)
    void AtStart();
    UFUNCTION(BlueprintImplementableEvent)
    void AtDead();
    UFUNCTION(BlueprintImplementableEvent)
	void DamageImpact();

private:
    class AMainCharacter* MainCharacter;
	class ALinearFPSPlayerController* MainPlayerController;
	void StartGameEvents();
    UPROPERTY(EditAnywhere, Category = "Begin Game Event")
    float BeginCount;
};
