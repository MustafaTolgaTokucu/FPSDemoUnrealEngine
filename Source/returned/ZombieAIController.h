// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ZombieAIController.generated.h"


//=======================
//enum for state machines
//=======================
UENUM(BlueprintType)
enum class EZombieState : uint8
{
    Idle UMETA(DisplayName = "Idle"),
	WakeUp UMETA(DisplayName = "WakeUp"),
    Chasing UMETA(DisplayName = "Chasing"),
    RotateToPlayer UMETA(DisplayName = "RotateToPlayer"),
    Attacking UMETA(DisplayName = "Attacking"),
	ItemThrowing UMETA(DisplayName=" ItemThrowing")
	//RotateToPlayer UMETA(DisplayName = "Rotate To Player"),
	
};


UCLASS()
class RETURNED_API AZombieAIController : public AAIController
{
	GENERATED_BODY()
public: 
	AZombieAIController();

	//=============================
	//State machine test functions
	//=============================
	bool PlayerInWakeRange();
	void ChasePlayer();
	bool PlayerInAttackRange();
	void Attack();
	bool ThrowableItemBlock();
	void ThrowItem();
	bool RotateToPlayer(float DeltaTime);
	FString GetStateAsString(EZombieState State);


protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	class AZombie* Zombie;
	//class AMainCharacter* MainCharacterHit;

private:
	class AMainCharacter* MainCharacter;
	class UAISenseConfig_Sight* SightConfig;
	class UAIPerceptionComponent* PerceptionComponent;
	void SetupPerceptionSystem();
	void UpdateSightConfig(float NewPeripheralVisionAngle);
	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus);
	bool bCanSee;
	//=========================
	//State test variables
	//=========================
	EZombieState CurrentState;

	FTimerHandle WakingUp;
	FTimerHandle AttackRate;
	FTimerHandle RotateRate;
	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	bool bWakingUp;
	bool bThrowing;
	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	bool bCanDamage;
	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	bool bChasing;
	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	bool bAttacking;
	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	bool bAttackAnim;
};
