// Fill out your copyright notice in the Description page of Project Settings.


#include "LinearFPSGameModeBase.h"
#include "Kismet/Gameplaystatics.h"
#include "MainCharacter.h"
#include "CombineTurret.h"
#include "Combine.h"
#include "HeadCrab.h"
#include "Zombie.h"
#include "Glock.h"
#include "LinearFPSPlayerController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CombineAIController.h"
void ALinearFPSGameModeBase::ActorDied(AActor *DeadActor, AActor* Killer)
{
if (DeadActor==MainCharacter)
{
    MainCharacter->DestructionHandler();
    if (MainPlayerController)
    {
        MainPlayerController->SetPlayerEnabledState(false);
        /*MainCharacter->DisableInput(MainCharacter->GetMainPlayerController());
        MainCharacter->GetMainPlayerController()->bShowMouseCursor = false;*/
        AtDead();
        UE_LOG(LogTemp, Error, TEXT("%s"), *Killer->GetActorNameOrLabel());
    }
    
}
else if (ACombineTurret* DestroyedTurret = Cast<ACombineTurret>(DeadActor))
    {
        DestroyedTurret->DestructionHandler();
    }
else if (ACombine* Combine =Cast<ACombine>(DeadActor))
    {
        Combine->DestructionHandler();
    }
else if (AHeadCrab* HeadCrab = Cast<AHeadCrab>(DeadActor))
    {
        HeadCrab->DestructionHandler();
    }
else if (AZombie* Zombie = Cast<AZombie>(DeadActor))
    {
        Zombie->DestructionHandler();
    }
    
}

void ALinearFPSGameModeBase::AtImpact(AActor* ImpactActor)
{
    if (ImpactActor==MainCharacter)
    {
        
        DamageImpact();

    }
   
   
}

void ALinearFPSGameModeBase::BeginPlay()
{

    Super::BeginPlay();
    StartGameEvents();
    
}

void ALinearFPSGameModeBase::StartGameEvents()
{
    AtStart();
    MainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(this,0));
    MainPlayerController = Cast<ALinearFPSPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
    if (MainPlayerController)
    {
        //Before timer player state is disabled
        MainPlayerController->SetPlayerEnabledState(false);
        //Timer handle to use it in set timer
        FTimerHandle PlayerEnableTimerHandle;
        //Delegate to trigger player state
        FTimerDelegate PlayerEnableTimerDelegate = FTimerDelegate::CreateUObject(MainPlayerController, &ALinearFPSPlayerController::SetPlayerEnabledState, true);
        //Countdown to trigger player state
        GetWorldTimerManager().SetTimer(PlayerEnableTimerHandle, PlayerEnableTimerDelegate, BeginCount, false);
    }
    






}
