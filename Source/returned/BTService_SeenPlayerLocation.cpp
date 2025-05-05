// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_SeenPlayerLocation.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"



UBTService_SeenPlayerLocation::UBTService_SeenPlayerLocation()
{
    NodeName=TEXT("Current Player Location");

}

void UBTService_SeenPlayerLocation::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
    APawn* MainCharacter=UGameplayStatics::GetPlayerPawn(GetWorld(),0);
    if (MainCharacter==nullptr)
    {
        return;
    }
    if (OwnerComp.GetAIOwner()==nullptr)
    {
        return;
    }
    
    
    OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(),MainCharacter->GetActorLocation());
            
        
    

    
}

