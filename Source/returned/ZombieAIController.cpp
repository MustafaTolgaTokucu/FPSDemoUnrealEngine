// Fill out your copyright notice in the Description page of Project Settings.

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "MainCharacter.h"
#include "Zombie.h"
#include "ZombieAIController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
AZombieAIController::AZombieAIController()
{
    PrimaryActorTick.bCanEverTick = true;
    SetupPerceptionSystem();
    CurrentState = EZombieState::Idle;
    bWakingUp=false;
    bThrowing=false;
    bCanDamage=false;
    bChasing = false;
    bAttacking = false;
    bAttackAnim = false;
}



void AZombieAIController::BeginPlay()
{
    Super::BeginPlay();
    MainCharacter =Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(this,0));
    Zombie = Cast<AZombie>(GetPawn());
    if (Zombie)
    {
        Zombie->GetCharacterMovement()->MaxWalkSpeed = 250.0f;
    }
    bCanDamage=false;
    
}

void AZombieAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    //==================
    //Conditional states
    //==================
    if (Zombie==nullptr)
    {
        return;
    }
    /*FVector DirectionToPlayer = MainCharacter->GetActorLocation() - Zombie->GetActorLocation();    
    Zombie->SetActorRotation(DirectionToPlayer.Rotation());*/
   // UE_LOG(LogTemp, Display, TEXT("Zombie"));
    //FString CurrentStateString = GetStateAsString(CurrentState);
    //UE_LOG(LogTemp, Warning, TEXT("Zombie Current State: %s"), *CurrentStateString);
    //UE_LOG(LogTemp, Warning, TEXT(" to blind %s"), IsOnGround()? TEXT("true"): TEXT("false"));
    UE_LOG(LogTemp, Warning, TEXT(" to blind %s"), bCanDamage ? TEXT("true"): TEXT("false"));
    
   
    switch (CurrentState)
    {
    case EZombieState::Idle:
        // Handle idle logic (e.g., patrolling)
        if (PlayerInWakeRange() && bCanSee)
        {      
            CurrentState = EZombieState::WakeUp;
            UpdateSightConfig(180.0f);
        }
        break;
    case EZombieState::WakeUp:
        // Handle chasing logic
        bWakingUp=true;
        if (!GetWorldTimerManager().IsTimerActive(WakingUp))
            {
                GetWorldTimerManager().SetTimer(WakingUp, [this]()
                {
                CurrentState = EZombieState::RotateToPlayer;
                bWakingUp=false;
                }, 2.5f, false);
            }

        break;

    case EZombieState::RotateToPlayer:
        if (RotateToPlayer(DeltaTime))
        {
             if (!GetWorldTimerManager().IsTimerActive(RotateRate))
            {
                GetWorldTimerManager().SetTimer(RotateRate, [this]()
                {
                CurrentState = EZombieState::Chasing;
                
                }, 1.f, false);
            }
            
        }
        break;   
    case EZombieState::Chasing:
        ChasePlayer();
        if (PlayerInAttackRange())
        {
            CurrentState = EZombieState::Attacking;
            break;
        }
        if (ThrowableItemBlock())
        {
            CurrentState = EZombieState::Attacking;
            break;
        }
        
        break;
    case EZombieState::Attacking:
        RotateToPlayer(DeltaTime);
        Attack();
        if (!PlayerInAttackRange()&&!bAttackAnim)
            {    
                bAttacking=false;
                CurrentState = EZombieState::Chasing;
                break;
            } 
        break;

    case EZombieState::ItemThrowing:
        // Handle attacking logic, possibly triggered by an animation notify
        ThrowItem();
        bThrowing=true;
        if (!GetWorldTimerManager().IsTimerActive(WakingUp))
            {
                GetWorldTimerManager().SetTimer(WakingUp, [this]()
                {
                CurrentState = EZombieState::Chasing;
                bThrowing=false;
                }, 0.6f, false);
            }
        break;
    }
}

void AZombieAIController::SetupPerceptionSystem()
{
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
    if (SightConfig)
    {
        UE_LOG(LogTemp, Warning, TEXT("Zombie Has Sight Config"));
        PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component"));
        SightConfig->SightRadius=5000;
        SightConfig->LoseSightRadius=5025.f;
        SightConfig->PeripheralVisionAngleDegrees = 180.0f;
        //SightConfig->AutoSuccessRangeFromLastSeenLocation=1030.f;
        SightConfig->DetectionByAffiliation.bDetectEnemies=true;
        SightConfig->DetectionByAffiliation.bDetectFriendlies=true;
        SightConfig->DetectionByAffiliation.bDetectNeutrals=true;
        PerceptionComponent->SetDominantSense(*SightConfig->GetSenseImplementation());
        PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this,&AZombieAIController::OnTargetDetected);
        PerceptionComponent->ConfigureSense(*SightConfig);
    }
}

void AZombieAIController::UpdateSightConfig(float NewPeripheralVisionAngle)
{
    if (SightConfig)
    {
        SightConfig->PeripheralVisionAngleDegrees = NewPeripheralVisionAngle;
        PerceptionComponent->RequestStimuliListenerUpdate(); // Force update perception
    }
}

void AZombieAIController::OnTargetDetected(AActor *Actor, FAIStimulus const Stimulus)
{
    if (Actor==MainCharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("Zombie can see"));
        bCanSee = Stimulus.WasSuccessfullySensed();
        //bToBlind = true;
        //GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", bCanSee); 
        
    }
}

bool AZombieAIController::PlayerInWakeRange()
{
    FVector ZombieLocation = Zombie->GetActorLocation();
    FVector PlayerLocation = MainCharacter->GetActorLocation();
    float Distance = FVector::Distance(ZombieLocation,PlayerLocation);
    if (Distance<300.f)
    {
        return true;
    }
    
    return false;
}

void AZombieAIController::ChasePlayer()
{
    bChasing = true;    
    MoveToLocation(MainCharacter->GetActorLocation());
}

bool AZombieAIController::PlayerInAttackRange()
{
    FVector ZombieLocation = Zombie->GetActorLocation();
    FVector PlayerLocation = MainCharacter->GetActorLocation();
    float Distance = FVector::Distance(ZombieLocation,PlayerLocation);
    if (Distance<100.f)
    {
        return true;
    }
    return false;
}

void AZombieAIController::Attack()
{
    bChasing=false;
    bAttacking = true;
    FVector ZombieStart = Zombie->GetActorLocation();
    //FVector ZombieForward = Zombie->GetActorForwardVector().GetSafeNormal();
    FVector ZombieForward = (MainCharacter->GetActorLocation()-ZombieStart).GetSafeNormal();
    FVector ZombieEnd = ZombieStart + ZombieForward*100; // Trace 50 units Forward

    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(Zombie); // Ignore self


    GetWorld()->LineTraceSingleByChannel(HitResult, ZombieStart, ZombieEnd, ECC_GameTraceChannel4, Params);
    /*GetWorld()->SweepSingleByChannel(
        HitResult,                      // Out hit result
        ZombieStart,                  // Start location of the trace
        ZombieEnd,                    // End location of the trace
        FQuat::Identity,                // Rotation of the trace (no rotation needed for a simple sphere)
        ECC_Visibility,                 // Collision channel to trace on (can be adjusted)
        FCollisionShape::MakeSphere(50.f), // Shape of the trace
        Params                     // Trace parameters
    );*/
    //DrawDebugLine(GetWorld(), ZombieStart, ZombieEnd, FColor::Red, true);
    //DrawDebugSphere(GetWorld(), ZombieEnd, 50.f, 32, FColor::Green, false,1.f);
    if (HitResult.GetActor())
    {
       UE_LOG(LogTemp, Warning, TEXT("Name: %s"),*HitResult.GetActor()->GetActorNameOrLabel()); 
    }
    
    if (AMainCharacter* MainCharacterHit = Cast<AMainCharacter>(HitResult.GetActor()) )
    {
        //UE_LOG(LogTemp, Warning, TEXT("character no damage"));
        if (bCanDamage)
        {
            UE_LOG(LogTemp, Warning, TEXT("Character damage"));
            
            
            UGameplayStatics::ApplyDamage(MainCharacterHit, 20.f, Zombie->GetController(), Zombie, UDamageType::StaticClass());
            bCanDamage = false;
            
        } 
        /*if (!bCanDamage&&bAttacking)
        {
        if (!GetWorldTimerManager().IsTimerActive(AttackRate))
            {
                GetWorldTimerManager().SetTimer(AttackRate, [this]()
                {
                bCanDamage = true;
                },1.f, false);
            }
        }*/
    }
    

}

bool AZombieAIController::ThrowableItemBlock()
{
    FVector ZombieStart = Zombie->GetActorLocation();
    FVector ZombieForward = Zombie->GetActorForwardVector().GetSafeNormal();
    FVector ZombieEnd = ZombieStart + ZombieForward*250; // Trace 50 units Forward

    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(Zombie); // Ignore self

    GetWorld()->LineTraceSingleByChannel(HitResult, ZombieStart, ZombieEnd, ECC_Visibility, Params);
    //DrawDebugLine(GetWorld(), ZombieStart, ZombieEnd, FColor::Red, true);
    if (HitResult.GetActor()&&HitResult.GetActor()->ActorHasTag("Throwable"))
    {
        UE_LOG(LogTemp, Warning, TEXT("Name: %s"),*HitResult.GetActor()->GetActorNameOrLabel());
        return true;
    }
    return false;
}

void AZombieAIController::ThrowItem()
{
}

bool AZombieAIController::RotateToPlayer(float DeltaTime)
{
    FRotator CurrentRotation = Zombie->GetActorRotation();
    FVector DirectionToPlayer = MainCharacter->GetActorLocation() - Zombie->GetActorLocation();    
    FRotator NewRotation = FMath::RInterpTo(CurrentRotation, DirectionToPlayer.Rotation(), DeltaTime, 6.0f);
    Zombie->SetActorRotation(NewRotation);
    return true;

}

FString AZombieAIController::GetStateAsString(EZombieState State)
{
     switch (State)
    {
    case EZombieState::Idle:
        return TEXT("Idle");
    case EZombieState::WakeUp:
        return TEXT("WakeUp");
    case EZombieState::Chasing:
        return TEXT("Chasing");
    case EZombieState::Attacking:
        return TEXT("Attacking");
    case EZombieState::ItemThrowing:
        return TEXT("ItemThrowing");
    default:
        return TEXT("Unknown");
    }
}
