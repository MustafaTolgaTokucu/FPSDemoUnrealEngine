// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_PathPointIncrement.h"
#include "Combine.h"
#include "HeadCrab.h"
#include "CombineAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_PathPointIncrement::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
     Super::ExecuteTask(OwnerComp, NodeMemory);
    // attemt tı get the combine controller
    if ((OwnerComp.GetAIOwner()))
    {
        // attemt to get the blackboard component from behaviour tree
        if (UBlackboardComponent* Blackboard= OwnerComp.GetBlackboardComponent())
        {
            
            // get combine
            ACombine* Combine = Cast<ACombine>(OwnerComp.GetAIOwner()->GetPawn());
            AHeadCrab* HeadCrab = Cast<AHeadCrab>(OwnerComp.GetAIOwner()->GetPawn());
            if (Combine)
            {
                //get the number of path points and min&max indices
                NumOfPoints = Combine->GetPatrolPath()->Num();
            }
            if (HeadCrab)
            {
                //get the number of path points and min&max indices
                NumOfPoints = HeadCrab->GetPatrolPath()->Num();
            }
            int MinIndex = 0;
            int MaxIndex = NumOfPoints-1;
            int Index = Blackboard->GetValueAsInt(GetSelectedBlackboardKey());
            //change direction if we are in the first or the last index if bidirectional is open
            if(bBiDirectional)
            {
                if (Index>=MaxIndex && Direction== EDirectionType::Forward)
                {
                    Direction= EDirectionType::Reverse;
                }
                else if (Index==MinIndex && Direction== EDirectionType::Reverse)
                {
                    Direction= EDirectionType::Forward;
                }
            }

            Blackboard->SetValueAsInt(GetSelectedBlackboardKey(), (Direction==EDirectionType::Forward ? ++Index : --Index) % NumOfPoints);

            return EBTNodeResult::Succeeded;
        }
        
    }
    
return EBTNodeResult::Failed;
}
