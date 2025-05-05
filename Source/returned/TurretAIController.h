// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TurretAIController.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ETurretState : uint8
{
    Idle UMETA(DisplayName = "Idle"),
	//WakeUp UMETA(DisplayName = "WakeUp"),
    //Chasing UMETA(DisplayName = "Chasing"),
    //RotateToPlayer UMETA(DisplayName = "RotateToPlayer"),
    Attacking UMETA(DisplayName = "Attacking"),
	Searching UMETA(DisplayName=" Searching")
	//RotateToPlayer UMETA(DisplayName = "Rotate To Player"),
	
};
UCLASS()
class RETURNED_API ATurretAIController : public AAIController
{
	GENERATED_BODY()
public:
	ATurretAIController();
	FString GetStateAsString(ETurretState State);
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
private:
	class AMainCharacter* MainCharacter;
	class ACombineTurret* Turret;
	ETurretState CurrentState;

	class UAISenseConfig_Sight* SightConfig;
	class UAIPerceptionComponent* PerceptionComponent;
	void SetupPerceptionSystem();
	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus);
	bool bCanSee;
	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	bool bSearching;
	bool bCanAttack;
	FTimerHandle SearchTime;
	FTimerHandle FireRate;

	void RotateToPlayer(float DeltaTime);
	void TurnForward(float DeltaTime);
	void Attack();
};
