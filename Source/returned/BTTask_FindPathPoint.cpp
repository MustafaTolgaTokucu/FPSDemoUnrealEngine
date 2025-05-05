// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPathPoint.h"
#include "Combine.h"
#include "HeadCrab.h"
#include "CombineAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PatrolPath.h"


UBTTask_FindPathPoint::UBTTask_FindPathPoint()
{
}

EBTNodeResult::Type UBTTask_FindPathPoint::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    // attemt tı get the combine controller
    if (OwnerComp.GetAIOwner())
    {
        // attemt to get the blackboard component from behaviour tree
        if (UBlackboardComponent* Blackboard= OwnerComp.GetBlackboardComponent())
        {
            //get the current patrol path index from the blackboard
            int PPIndex = Blackboard->GetValueAsInt(GetSelectedBlackboardKey());
            // get combine
            ACombine* Combine = Cast<ACombine>(OwnerComp.GetAIOwner()->GetPawn());
            AHeadCrab* HeadCrab = Cast<AHeadCrab>(OwnerComp.GetAIOwner()->GetPawn());
            if (Combine)
            {
                //get the current patrol path vector from combine - local vector
                if (Combine->GetPatrolPath()==nullptr)
                {
                    return EBTNodeResult::Failed;
                }
                FVector PatrolPathPoint = Combine->GetPatrolPath()->GetPatrolPoint(PPIndex);
                // convert from local vector to global point
                auto GlobalPoint = Combine->GetPatrolPath()->GetActorTransform().TransformPosition(PatrolPathPoint);
                Blackboard->SetValueAsVector(PatrolPathVectorKey.SelectedKeyName, GlobalPoint);
                return EBTNodeResult::Succeeded;
            }
            if(HeadCrab)
            {
                if (HeadCrab->GetPatrolPath()==nullptr)
                {
                    return EBTNodeResult::Failed;
                }
                FVector PatrolPathPoint = HeadCrab->GetPatrolPath()->GetPatrolPoint(PPIndex);
                // convert from local vector to global point
                auto GlobalPoint = HeadCrab->GetPatrolPath()->GetActorTransform().TransformPosition(PatrolPathPoint);
                Blackboard->SetValueAsVector(PatrolPathVectorKey.SelectedKeyName, GlobalPoint);
                return EBTNodeResult::Succeeded;
            }
        }
    }
    
return EBTNodeResult::Failed;
    
}


