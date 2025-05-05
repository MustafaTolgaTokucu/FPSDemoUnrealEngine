// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_ChangeWalkingSpeed.generated.h"

/**
 * 
 */
UCLASS()
class RETURNED_API UBTService_ChangeWalkingSpeed : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTService_ChangeWalkingSpeed();
	virtual void TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed = 600.f;
};
