// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerLocation.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_PlayerLocation::UBTService_PlayerLocation()
{
    NodeName = TEXT("Update Player Last Location");
}

void UBTService_PlayerLocation::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
    APawn* MainCharacter=UGameplayStatics::GetPlayerPawn(GetWorld(),0);
        if (MainCharacter==nullptr)
        {
            return;
        }
    OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(),MainCharacter->GetActorLocation());
    
}
