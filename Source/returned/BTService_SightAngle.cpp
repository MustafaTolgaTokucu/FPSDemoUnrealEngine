// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_SightAngle.h"
#include "CombineAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Combine.h"

UBTService_SightAngle::UBTService_SightAngle()
{
    NodeName= "Combine Sight Angle";
}

void UBTService_SightAngle::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{
    if (ACombineAIController* CombineController = Cast<ACombineAIController>(OwnerComp.GetAIOwner()))
    { 
        CombineController->SightAngle=SightAngleServ;
        //CombineController->GetBlackboardComponent()->SetValueAsFloat("CombineSightAngle", SightAngleServ);
        
    }
}
