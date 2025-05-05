// Fill out your copyright notice in the Description page of Project Settings.

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "MainCharacter.h"
#include "CombineTurret.h"
#include "Kismet/GameplayStatics.h"
#include "TurretAIController.h"

ATurretAIController::ATurretAIController()
{
    SetupPerceptionSystem();
    CurrentState = ETurretState::Idle;
    bCanSee =false;
    bSearching = false;
    bCanAttack = true;
}



void ATurretAIController::BeginPlay()
{
    Super::BeginPlay();
    MainCharacter =Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(this,0));
    Turret = Cast<ACombineTurret>(GetPawn());


}

void ATurretAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    /*FRotator BodyRotation = Turret->BodyPivot->GetRelativeRotation();
  
    UE_LOG(LogTemp, Warning, TEXT("%s"),*BodyRotation.ToString());*/

    /*FString CurrentStateString = GetStateAsString(CurrentState);
    UE_LOG(LogTemp, Warning, TEXT("Turret Current State: %s"), *CurrentStateString);*/
    switch (CurrentState)
    {
    case ETurretState::Idle:
        // Handle idle logic (e.g., patrolling)
        TurnForward(DeltaTime);
        bSearching =false;
        if (bCanSee)
        {
            CurrentState = ETurretState::Attacking;
        }
        
        break;
    /*case ETurretState::RotateToPlayer:
        // Handle chasing logic
        if (RotateToPlayer())
        {
            CurrentState = ETurretState::Attacking;
        }
        

        break;*/

    case ETurretState::Attacking:
        RotateToPlayer(DeltaTime);
        Attack();
        if (!bCanSee)
        {
            
            CurrentState = ETurretState::Searching;
            
        }
        
        break;   
    case ETurretState::Searching:
       bSearching=true;
       TurnForward(DeltaTime);
       if (!GetWorldTimerManager().IsTimerActive(SearchTime))
            {
                GetWorldTimerManager().SetTimer(SearchTime, [this]()
                {
                CurrentState = ETurretState::Idle;
                bSearching=false;
                }, 2.f, false);
            }
        if (bCanSee)
        {
            bSearching=false;
            
            CurrentState = ETurretState::Attacking;
        }
        
        break;
    
    }

}

void ATurretAIController::SetupPerceptionSystem()
{
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
    if (SightConfig)
    {
        UE_LOG(LogTemp, Warning, TEXT("Turret Has Sight Config"));
        PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component"));
        SightConfig->SightRadius=5000;
        SightConfig->LoseSightRadius=5025.f;
        SightConfig->PeripheralVisionAngleDegrees = 75.f;
        //SightConfig->AutoSuccessRangeFromLastSeenLocation=1030.f;
        SightConfig->DetectionByAffiliation.bDetectEnemies=true;
        SightConfig->DetectionByAffiliation.bDetectFriendlies=true;
        SightConfig->DetectionByAffiliation.bDetectNeutrals=true;
        
        PerceptionComponent->SetDominantSense(*SightConfig->GetSenseImplementation());
        PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this,&ATurretAIController::OnTargetDetected);
        PerceptionComponent->ConfigureSense(*SightConfig);
    }
}

void ATurretAIController::OnTargetDetected(AActor *Actor, FAIStimulus const Stimulus)
{
    if (Actor==MainCharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("Turret can see"));
        bCanSee = Stimulus.WasSuccessfullySensed();
        //bToBlind = true;
        //GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", bCanSee); 
        
    }
}

void ATurretAIController::RotateToPlayer(float DeltaTime)
{
    

    // Get the location of the player and the turret body
    FVector PlayerLocation = MainCharacter->GetActorLocation();
    FVector TurretBodyLocation = Turret->BodyPivot->GetComponentLocation();

    // Calculate the direction to the player
    FVector Direction = (PlayerLocation - TurretBodyLocation).GetSafeNormal();
    FRotator LookAtRotation = FRotationMatrix::MakeFromX(Direction).Rotator();

    FRotator BaseRotation = Turret->GetActorRotation();

    // We only want to rotate around the Yaw (Z axis)
    FRotator BodyRotation = Turret->BodyPivot->GetRelativeRotation();
    // Convert the world LookAtRotation to the local space of the body pivot
    FRotator LocalLookAtRotation = LookAtRotation - BaseRotation;
    //UE_LOG(LogTemp, Warning, TEXT("%s"),*BodyRotation.ToString());
    //BodyRotation.Yaw = LookAtRotation.Yaw;
    //DrawDebugLine(GetWorld(), TurretBodyLocation, (TurretBodyLocation+Direction*200), FColor::Red,1 ,false);
    // Apply the new rotation to the body
    //FRotator ToRotate = FRotator (BodyRotation.Pitch,LookAtRotation.Yaw,BodyRotation.Roll);
    //FRotator NewRotation = FMath::RInterpTo(BodyRotation, FRotator(0.f,LookAtRotation.Yaw,90.f), DeltaTime, 6.f);
    FRotator NewRotation = FMath::RInterpTo(BodyRotation, FRotator(0.f,LocalLookAtRotation.Yaw,90.f), DeltaTime, 6.f);
    Turret->BodyPivot->SetRelativeRotation(NewRotation);

    FVector HeadLocation = Turret->HeadPivot->GetComponentLocation();
    FVector HeadDirection = (PlayerLocation - HeadLocation).GetSafeNormal();
    FRotator HeadLookAtRotation = FRotationMatrix::MakeFromX(HeadDirection).Rotator();
    FRotator HeadRotation = Turret->HeadPivot->GetRelativeRotation();
    HeadRotation.Pitch = HeadLookAtRotation.Pitch;

    // Apply the new rotation to the head
    Turret->HeadPivot->SetRelativeRotation(HeadRotation);

}

void ATurretAIController::TurnForward(float DeltaTime)
{
    FRotator BodyRotation = Turret->BodyPivot->GetRelativeRotation();
    FRotator HeadRotation = Turret->HeadPivot->GetRelativeRotation();
    //BodyRotation.Yaw = 0.f;
    HeadRotation.Pitch = 0.f;
    //Turret->BodyPivot->SetRelativeRotation(BodyRotation);
    Turret->HeadPivot->SetRelativeRotation(HeadRotation);
    FRotator NewRotation = FMath::RInterpTo(BodyRotation, FRotator(0.f,0.f,90.f), DeltaTime, 5.f);
    Turret->BodyPivot->SetRelativeRotation(NewRotation);
}

void ATurretAIController::Attack()
{
    FVector PlayerLocation = MainCharacter->GetActorLocation();
    //FVector TurretForward = Turret->SpawnP->GetForwardVector();
    FVector TurretStart = Turret->SpawnP->GetComponentLocation();
    FVector ToDirection = (PlayerLocation - TurretStart).GetSafeNormal();
    //FVector TurretEnd = TurretStart + TurretForward*2500;
    float SpreadDegree=7.f;
    float SpreadAngle = FMath::DegreesToRadians(SpreadDegree);
    
    FVector RandomDirection = FMath::VRandCone(ToDirection, SpreadAngle);
    FVector TurretEnd = TurretStart + (RandomDirection * 2500.f);
    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(Turret);
    //DrawDebugLine(GetWorld(), TurretStart, TurretEnd, FColor::Red, true);
    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, TurretStart, TurretEnd, ECC_GameTraceChannel4, Params);
    
    if (bHit)
    {
        // Apply damage
        AActor* HitActor = HitResult.GetActor();
        if (HitActor)
        {
        auto DamageType = UDamageType::StaticClass();
        
        if (bCanAttack)
        {
            UGameplayStatics::ApplyPointDamage(HitActor, 4.f, RandomDirection, HitResult, Turret->GetController(), this, DamageType);
            bCanAttack = false;
        }
        
        
        if (!GetWorldTimerManager().IsTimerActive(FireRate))
            {
                GetWorldTimerManager().SetTimer(FireRate, [this]()
                {
                    bCanAttack=true;
                    //UE_LOG(LogTemp, Warning, TEXT("%s"),*HitActor->GetActorNameOrLabel());
                    //UE_LOG(LogTemp, Warning, TEXT("Combine hit"));
                    //UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Muzzle);
                    //UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, HitResult.ImpactPoint);
                }, 0.25f, false);
            }
        
        }
    }
}
FString ATurretAIController::GetStateAsString(ETurretState State)
{
    switch (State)
    {
    case ETurretState::Idle:
        return TEXT("Idle");
    /*case EZombieState::WakeUp:
        return TEXT("WakeUp");
    case EZombieState::Chasing:
        return TEXT("Chasing");*/
    case ETurretState::Attacking:
        return TEXT("Attacking");
    case ETurretState::Searching:
        return TEXT("Searching");
    default:
        return TEXT("Unknown");
    }
    
}