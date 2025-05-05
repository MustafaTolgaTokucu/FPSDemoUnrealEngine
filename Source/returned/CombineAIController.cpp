// Fill out your copyright notice in the Description page of Project Settings.


#include "CombineAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MainCharacter.h"

ACombineAIController::ACombineAIController()
{
    SetupPerceptionSystem();
}

void ACombineAIController::BeginPlay()
{
    Super::BeginPlay();

    MainCharacter =Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(this,0));

    if (BTCombine)
    {
        RunBehaviorTree(BTCombine);
        GetBlackboardComponent()->SetValueAsVector(TEXT("CombineStartLocation"),GetPawn()->GetActorLocation());
        GetBlackboardComponent()->SetValueAsBool(TEXT("IsAlive?"),true);
        GetBlackboardComponent()->SetValueAsObject("MainCharacter",MainCharacter);
    }
    
}

void ACombineAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
   
    bCombineShooting = GetBlackboardComponent()->GetValueAsBool("Shooting");
    
    //UE_LOG(LogTemp, Warning, TEXT("%f"),SightAngle);
   

}

void ACombineAIController::SetupPerceptionSystem()
{
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
    
    if (SightConfig)
    {
        //UE_LOG(LogTemp, Warning, TEXT("Has Sight Config"));
        PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component"));
        SightConfig->SightRadius=5000;
        SightConfig->LoseSightRadius=5050.f;
        SightConfig->PeripheralVisionAngleDegrees = 90.f;
        //SightConfig->AutoSuccessRangeFromLastSeenLocation=1030.f;
        SightConfig->DetectionByAffiliation.bDetectEnemies=true;
        SightConfig->DetectionByAffiliation.bDetectFriendlies=true;
        SightConfig->DetectionByAffiliation.bDetectNeutrals=true;
        PerceptionComponent->SetDominantSense(*SightConfig->GetSenseImplementation());
        PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this,&ACombineAIController::OnTargetDetected);
        PerceptionComponent->ConfigureSense(*SightConfig);
    }
    
}

void ACombineAIController::OnTargetDetected(AActor *Actor, FAIStimulus const Stimulus)
{
 if (Actor==MainCharacter)
    {
        
        bCanSee = Stimulus.WasSuccessfullySensed();
        GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", bCanSee);   
    }
}
void ACombineAIController::FocusToPlayer()
{
    if (GetPawn()==nullptr)
    {
        return;
    }
    
    FVector DirectionToPlayer = MainCharacter->GetActorLocation() - GetPawn()->GetActorLocation();    
    GetPawn()->SetActorRotation(DirectionToPlayer.Rotation());

}