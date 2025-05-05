// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "HeadCrabAIController.generated.h"

/**
 * 
 */
//=======================
//enum for state machines
//=======================
UENUM(BlueprintType)
enum class EHeadcrabState : uint8
{
    Idle UMETA(DisplayName = "Idle"),
    Chasing UMETA(DisplayName = "Chasing"),
    Jumping UMETA(DisplayName = "Jumping"),
    Attacking UMETA(DisplayName = "Attacking"),
	RotateToPlayer UMETA(DisplayName = "Rotate To Player"),
	Landing UMETA(DisplayName="Landing")
};


UCLASS()
class RETURNED_API AHeadCrabAIController : public AAIController
{
	GENERATED_BODY()
public: 
	AHeadCrabAIController();

	//=============================
	//State machine test functions
	//=============================
	void OnPlayerDetected();
    void LaunchTowardsPlayer();
    void CheckProximityAndDamage();
    void OnLandingDetected();
    bool IsNearPlayerHead();
    bool IsOnGround();
	bool IsRotated(float DeltaTime);
	void Patrol();
	bool CanSeePlayer();
	void UpdateSightConfig(float NewPeripheralVisionAngle);
	

	FString GetStateAsString(EHeadcrabState State);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnPossess(APawn* InPawn) override;

private:
	
	class AMainCharacter* MainCharacter;
	//==================
	//Perception for NPC
	//==================
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* BTHeadCrab;
	
	class UAISenseConfig_Sight* SightConfig;
	class UAIPerceptionComponent* PerceptionComponent;
	void SetupPerceptionSystem();
	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus);
	bool bCanSee;
	bool bComingFar;
	bool bToBlind;
	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	bool bJumping;
	bool bIsPossessed;



	//test
	FVector CharacterLocation;
	class AHeadCrab* HeadCrab;
	FTimerHandle AttackRate;
	FTimerHandle JumpRate;
	bool CanJump;
	//=========================
	//State test variables
	//=========================
	EHeadcrabState CurrentState;
    bool bHasJumped;
    float LastDamageTime;
    float DamageInterval;
    float JumpForce;
    float DamageAmount;
	bool bCandamage;

	FVector PatrolPathPoint;
	int NumOfPoints;
	int MaxIndex;
	int CurrentIndex;
	bool Forward;

	FVector HeadCrabStartLocation;
	FVector HeadCrabEndLocation;
	FVector HeadCrabForward;
};
