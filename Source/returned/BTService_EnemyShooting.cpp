// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_EnemyShooting.h"
#include "CombineAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Combine.h"

UBTService_EnemyShooting::UBTService_EnemyShooting()
{
    NodeName = "EnemyShooting";
}

void UBTService_EnemyShooting::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

   /*if (ACombineAIController* CombineController = Cast<ACombineAIController>(OwnerComp.GetAIOwner()))
    { 
            // get combine
            if (ACombine* Combine = Cast<ACombine>(CombineController->GetPawn()))
            {
              UBlackboardComponent* Blackboard = CombineController->GetBlackboardComponent();
                if (Blackboard)
                {
                    Blackboard->SetValueAsBool("Shooting", bShooting);
                    
                }
                
                
            }
            
        
        
    }*/
   if (OwnerComp.GetAIOwner())
    { 
            // get combine
            if (OwnerComp.GetAIOwner()->GetPawn())
            {
              UBlackboardComponent* Blackboard = OwnerComp.GetAIOwner()->GetBlackboardComponent();
                if (Blackboard)
                {
                    Blackboard->SetValueAsBool("Shooting", bShooting);
                    
                }  
            }    
    }
}
