// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_ChangeWalkingSpeed.h"
#include "Combine.h"
#include "HeadCrab.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CombineAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_ChangeWalkingSpeed::UBTService_ChangeWalkingSpeed()
{
    NodeName = "Change Walking Speed";
}

void UBTService_ChangeWalkingSpeed::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

   if (OwnerComp.GetAIOwner())
    { 
            // get combine
            ACombine* Combine = Cast<ACombine>(OwnerComp.GetAIOwner()->GetPawn());
            AHeadCrab* HeadCrab = Cast<AHeadCrab>(OwnerComp.GetAIOwner()->GetPawn());
            if (Combine)
            {
                Combine->GetCharacterMovement()->MaxWalkSpeed = Speed;   
            }
            if(HeadCrab)
            {
                HeadCrab->GetCharacterMovement()->MaxWalkSpeed = Speed;
            }
    }
    
}
