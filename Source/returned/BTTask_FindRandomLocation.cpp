// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindRandomLocation.h"
#include "Combine.h"
#include "CombineAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

UBTTask_FindRandomLocation::UBTTask_FindRandomLocation()
{
    NodeName = "Find Random Location";
}

EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{

    if (ACombineAIController* CombineController = Cast<ACombineAIController>(OwnerComp.GetAIOwner()))
    {
        // get combine
        if (ACombine* Combine = Cast<ACombine>(CombineController->GetPawn()))
        {
            //get the current location
            FVector OriginVector = Combine->GetActorLocation();
            // get the navigation system

            if (UNavigationSystemV1* NavigationSystem = UNavigationSystemV1::GetCurrent(GetWorld()))
            {
                FNavLocation NavLoc;
                if (NavigationSystem->GetRandomPointInNavigableRadius(OriginVector, SearchRadius, NavLoc))
                {
                    if (UBlackboardComponent* Blackboard= OwnerComp.GetBlackboardComponent())
                    {
                    Blackboard->SetValueAsVector(GetSelectedBlackboardKey(), NavLoc.Location);
                    }
                }
                return EBTNodeResult::Succeeded;
            }  
        }    
        
    }

    return EBTNodeResult::Failed;
}


