// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_EnemyShooting.generated.h"

/**
 * 
 */
UCLASS()
class RETURNED_API UBTService_EnemyShooting : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public: 
	UBTService_EnemyShooting();
	virtual void TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShooting = false;

};
