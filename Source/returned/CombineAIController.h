// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "CombineAIController.generated.h"

/**
 * 
 */
UCLASS()
class RETURNED_API ACombineAIController : public AAIController
{
	GENERATED_BODY()
	ACombineAIController();
public:
	UPROPERTY(BlueprintReadWrite)
	bool bCanSee = false;
	UPROPERTY(BlueprintReadWrite)
	bool bCombineShooting;
	void FocusToPlayer();
	float SightAngle=90.f;
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;



private:
	class AMainCharacter* MainCharacter;

	UPROPERTY(EditAnywhere)
	class UBehaviorTree* BTCombine;
	
	
	class UAISenseConfig_Sight* SightConfig;
	class UAIPerceptionComponent* PerceptionComponent;
	void SetupPerceptionSystem();
	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus);
};
