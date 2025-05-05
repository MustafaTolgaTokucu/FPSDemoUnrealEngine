// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Shoot.h"
#include "Combine.h"
#include "HeadCrab.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Shoot::UBTTask_Shoot()
{
    NodeName = TEXT("Shoot");
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    if (OwnerComp.GetAIOwner()==nullptr)
    {
        return EBTNodeResult::Failed;
    }
    
    ACombine* Combine = Cast<ACombine>(OwnerComp.GetAIOwner()->GetPawn());
    AHeadCrab* HeadCrab = Cast<AHeadCrab>(OwnerComp.GetAIOwner()->GetPawn());
    
    
    if (Combine)
    {
        Combine->Attack();
        return EBTNodeResult::Succeeded;
    }
    if (HeadCrab)
    {
        //HeadCrab->Attack();
        return EBTNodeResult::Succeeded;
    
    }
    
    if (OwnerComp.GetBlackboardComponent())
        {
            OwnerComp.GetBlackboardComponent()->SetValueAsBool("Shooting", true);
            return EBTNodeResult::Succeeded;
        }

    return EBTNodeResult::Succeeded;
}
