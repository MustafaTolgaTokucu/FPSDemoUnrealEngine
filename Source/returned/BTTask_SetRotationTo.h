// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_SetRotationTo.generated.h"

/**
 * 
 */
UCLASS()
class RETURNED_API UBTTask_SetRotationTo : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_SetRotationTo();
	
protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
