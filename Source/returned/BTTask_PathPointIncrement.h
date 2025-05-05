// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_PathPointIncrement.generated.h"

/**
 * 
 */
UCLASS()
class RETURNED_API UBTTask_PathPointIncrement : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
private: 
	enum class EDirectionType {Forward, Reverse};
	int NumOfPoints;
	EDirectionType Direction = EDirectionType::Forward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta=(AllowPrivateAccess="true"))
	bool bBiDirectional = false;
};
