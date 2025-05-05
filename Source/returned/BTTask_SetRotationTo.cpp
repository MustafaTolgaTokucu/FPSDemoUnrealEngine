// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetRotationTo.h"
#include "HeadCrab.h"
#include "BehaviorTree/BlackboardComponent.h"
UBTTask_SetRotationTo::UBTTask_SetRotationTo(){
    NodeName = "Set Rotation To";
    }

EBTNodeResult::Type UBTTask_SetRotationTo::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{

    Super::ExecuteTask(OwnerComp, NodeMemory);
    if (OwnerComp.GetAIOwner()==nullptr)
    {
        return EBTNodeResult::Failed;
    }
    
    //ACombine* Combine = Cast<ACombine>(OwnerComp.GetAIOwner()->GetPawn());
    AHeadCrab* HeadCrab = Cast<AHeadCrab>(OwnerComp.GetAIOwner()->GetPawn());
    
    if (HeadCrab)
    {
        //HeadCrab->RotationToPlayer();
        
        /*if (OwnerComp.GetBlackboardComponent())
        {
            OwnerComp.GetBlackboardComponent()->SetValueAsBool("RotationSet", true);
            return EBTNodeResult::Succeeded;
        }*/
        
    
        
        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Succeeded;


    
}
