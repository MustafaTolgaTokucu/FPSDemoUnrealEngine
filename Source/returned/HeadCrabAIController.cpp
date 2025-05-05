// Fill out your copyright notice in the Description page of Project Settings.


#include "HeadCrabAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/ArrowComponent.h"
#include "MainCharacter.h"
#include "HeadCrab.h"
#include "GameFramework/CharacterMovementComponent.h"

AHeadCrabAIController::AHeadCrabAIController()
{
    CurrentState = EHeadcrabState::Idle;
    SetupPerceptionSystem();
    CanJump=true;
    bCandamage = true;
    //bToBlind=false;
    bJumping = false;
    bIsPossessed = false;
}

void AHeadCrabAIController::BeginPlay()
{
    Super::BeginPlay();
    MainCharacter =Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(this,0));
    HeadCrab = Cast<AHeadCrab>(GetPawn());
    if (HeadCrab&&HeadCrab->GetPatrolPath())
    {
        NumOfPoints = HeadCrab->GetPatrolPath()->Num();
    }
    MaxIndex = NumOfPoints-1;
    CurrentIndex=0;
    Forward=true;
}

void AHeadCrabAIController::Tick(float DeltaTime)
{
    //Super::Tick(DeltaTime);
    
    //==================
    //Conditional states
    //==================
    FString CurrentStateString = GetStateAsString(CurrentState);
    //UE_LOG(LogTemp, Warning, TEXT("Headcrab Current State: %s"), *CurrentStateString);
    //UE_LOG(LogTemp, Warning, TEXT(" to blind %s"), IsOnGround()? TEXT("true"): TEXT("false"));
    //UE_LOG(LogTemp, Warning, TEXT(" to blind %s"), bIsPossessed ? TEXT("true"): TEXT("false"));
    //UE_LOG(LogTemp, Display, TEXT("fdsa"));
    if (bIsPossessed)
    {
        //UpdateSightConfig(180.0f);
        bCanSee=true;
        
        
        bIsPossessed = false;
    }
    

    switch (CurrentState)
    {
    case EHeadcrabState::Idle:
        // Handle idle logic (e.g., patrolling)
        if (CanSeePlayer())
        {      
            
            Patrol();
            UpdateSightConfig(75.0f);
            
            //UE_LOG(LogTemp, Error, TEXT("patrolling")); 
        }
        
        if (bCanSee)
        {
            //bToBlind = true;
            UpdateSightConfig(180.0f);
            CurrentState = EHeadcrabState::Chasing;
        }
        break;
    case EHeadcrabState::Chasing:
        // Handle chasing logic
        if (CanSeePlayer())
        {
            CurrentState = EHeadcrabState::Idle;
            break;
        }
        
        IsNearPlayerHead(); //State may change to attacking according to range    
        if (IsNearPlayerHead())  // If within jump range
        {
            CurrentState = EHeadcrabState::RotateToPlayer;
            
        }
        break;

    case EHeadcrabState::RotateToPlayer:
        if (IsRotated(DeltaTime))
        {
            if (bComingFar)
            {
                if (!GetWorldTimerManager().IsTimerActive(JumpRate))
                {
                    GetWorldTimerManager().SetTimer(JumpRate, [this]()
                    {
                    CurrentState = EHeadcrabState::Jumping;
                    },0.15f, false);
                }  
                break;
            }
            
            if (!GetWorldTimerManager().IsTimerActive(JumpRate))
            {
                GetWorldTimerManager().SetTimer(JumpRate, [this]()
                {
                CurrentState = EHeadcrabState::Jumping;
                }, 1.0f, false);
            }  
        }
        break;
    case EHeadcrabState::Jumping:
        // Handle jumping logic
        LaunchTowardsPlayer();
        CheckProximityAndDamage();
        //CurrentState = EHeadcrabState::Landing;
        if (!GetWorldTimerManager().IsTimerActive(JumpRate))
            {
                GetWorldTimerManager().SetTimer(JumpRate, [this]()
                {
                CurrentState = EHeadcrabState::Landing;
                }, 1.0f, false);
            } 
        break;

    case EHeadcrabState::Attacking:
        // Handle attacking logic, possibly triggered by an animation notify
        CheckProximityAndDamage();
        IsNearPlayerHead();
        if (IsNearPlayerHead())
        {
            CurrentState = EHeadcrabState::RotateToPlayer;
            LaunchTowardsPlayer();
        }
        break;

    case EHeadcrabState::Landing:
        // Handle attacking logic, possibly triggered by an animation notify
        
        OnLandingDetected();
        EHeadcrabState::Idle;
        

        break;
    }
    
}

void AHeadCrabAIController::OnPossess(APawn *InPawn)
{
    Super::OnPossess(InPawn);

    HeadCrab = Cast<AHeadCrab>(InPawn);
    bIsPossessed = true;
    
}

void AHeadCrabAIController::SetupPerceptionSystem()
{
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
    if (SightConfig)
    {
        UE_LOG(LogTemp, Warning, TEXT("HeadCrab Has Sight Config"));
        PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component"));
        SightConfig->SightRadius=5000;
        SightConfig->LoseSightRadius=5025.f;
        //SightConfig->PeripheralVisionAngleDegrees = 60.0f;
        //SightConfig->AutoSuccessRangeFromLastSeenLocation=1030.f;
        SightConfig->DetectionByAffiliation.bDetectEnemies=true;
        SightConfig->DetectionByAffiliation.bDetectFriendlies=true;
        SightConfig->DetectionByAffiliation.bDetectNeutrals=true;
        PerceptionComponent->SetDominantSense(*SightConfig->GetSenseImplementation());
        PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this,&AHeadCrabAIController::OnTargetDetected);
        PerceptionComponent->ConfigureSense(*SightConfig);
    }
}

void AHeadCrabAIController::OnTargetDetected(AActor *Actor, FAIStimulus const Stimulus)
{
    if (Actor==MainCharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("Headcrab can see"));
        bCanSee = Stimulus.WasSuccessfullySensed();
        bToBlind = true;
        //GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", bCanSee); 
        
    }
    
}

void AHeadCrabAIController::LaunchTowardsPlayer()
{
    
    
    if (HeadCrab==nullptr)
    {
        return;
    }
     if (MainCharacter==nullptr)
    {
        return;
    }
    HeadCrabStartLocation = HeadCrab->GetMesh()->GetComponentLocation();
    FVector TargetLocation = MainCharacter->JumpLocation->GetComponentLocation();
    // Calculate the difference in locations
    FVector ToTarget = TargetLocation - HeadCrabStartLocation;

    // Break down the vector into horizontal (XY plane) and vertical (Z) components
    FVector HorizontalDirection = FVector(ToTarget.X, ToTarget.Y, 0.0f);
    float HorizontalDistance = HorizontalDirection.Size();
    float VerticalDistance = ToTarget.Z;

    // Desired travel time (you can adjust this)
    float TimeToTarget = 0.5f;

    // Gravity (usually 980 for Unreal units)
    float Gravity = GetWorld()->GetGravityZ();

    // Calculate the necessary launch speed in the horizontal direction
    FVector HorizontalVelocity = HorizontalDirection.GetSafeNormal() * (HorizontalDistance / TimeToTarget);

    // Calculate the necessary launch speed in the vertical direction
    float VerticalVelocity = (VerticalDistance / TimeToTarget) - (0.75f * Gravity * TimeToTarget);

    // Combine both horizontal and vertical velocities to get the final launch velocity
    FVector LaunchVelocity = HorizontalVelocity;
    LaunchVelocity.Z = VerticalVelocity;
    bComingFar=false;
    
    if (CanJump)
    {
        // Launch the character with the calculated velocity
        HeadCrab->LaunchCharacter(LaunchVelocity, true, true);
        CanJump=false;
        bJumping = true;
        // Start the timer only when CanJump is set to false
        if (!GetWorldTimerManager().IsTimerActive(AttackRate))
        {
            GetWorldTimerManager().SetTimer(AttackRate, [this]()
            {
            CanJump = true;
            }, 3.f, false);
        }
    }
    
    
}

void AHeadCrabAIController::CheckProximityAndDamage()
{
    //UE_LOG(LogTemp, Error, TEXT("proximity work"));
     if (HeadCrab==nullptr)
    {
        return;
    }
    HeadCrabStartLocation = HeadCrab->GetActorLocation();
    HeadCrabForward = HeadCrab->GetActorForwardVector().GetSafeNormal();
    HeadCrabEndLocation = HeadCrabStartLocation + HeadCrabForward*100; // Trace 50 units Forward

    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this); // Ignore self

    GetWorld()->LineTraceSingleByChannel(HitResult, HeadCrabStartLocation, HeadCrabEndLocation, ECC_Visibility, Params);
    //DrawDebugLine(GetWorld(), HeadCrabStartLocation, HeadCrabEndLocation, FColor::Red, true);
    /*if (HitResult.GetActor())
    {
       //UE_LOG(LogTemp, Warning, TEXT("Name: %s"),*HitResult.GetActor()->GetActorNameOrLabel()); 
    }*/
    
    if (AMainCharacter* MainCharacterHit = Cast<AMainCharacter>(HitResult.GetActor()))
    {
        //UE_LOG(LogTemp, Warning, TEXT("character no damage"));
        if (bCandamage)
        {
            UE_LOG(LogTemp, Warning, TEXT("Character damage"));
            UGameplayStatics::ApplyDamage(MainCharacterHit, 20.f, HeadCrab->GetController(), HeadCrab, UDamageType::StaticClass());
            bCandamage = false;
            
        }     
    }
}

void AHeadCrabAIController::OnLandingDetected()
{
    bCandamage=true;
    
    bToBlind = false;
    bJumping = false;
    CurrentState = EHeadcrabState::Idle;
}

bool AHeadCrabAIController::IsNearPlayerHead()
{
    if (HeadCrab==nullptr)
    {
        return false;
    }
    CharacterLocation = MainCharacter->GetActorLocation();
    float Distance = FVector::Dist(CharacterLocation, HeadCrab->GetActorLocation());
    //CurrentState = EHeadcrabState::Jumping;
    /*if (Distance<50)
    {
        //CurrentState = EHeadcrabState::Attacking;
        //return false;
    }*/
    
    if (Distance<325)
    {
        FVector DirectionToPlayer = (HeadCrab->GetActorLocation() - CharacterLocation).GetSafeNormal();
        MoveToLocation(CharacterLocation+(DirectionToPlayer*375));
        //return false;
    }
    if (Distance>450)
    {
        FVector DirectionToPlayer = (HeadCrab->GetActorLocation() - CharacterLocation).GetSafeNormal();
        //FVector DirectionToPlayer = (CharacterLocation - HeadCrab->GetActorLocation()).GetSafeNormal();
        MoveToLocation(CharacterLocation+(DirectionToPlayer*400));
        bComingFar = true;
        //return false;
     
    }
    if (Distance>325&&Distance<450)
    {
        HeadCrab->SetActorLocation(HeadCrab->GetActorLocation());
        return true;
    }
    return false;
    
}

bool AHeadCrabAIController::IsOnGround()
{
     if (HeadCrab==nullptr)
    {
        return false;
    }
    HeadCrabStartLocation = HeadCrab->GetActorLocation();
    HeadCrabEndLocation = HeadCrabStartLocation - FVector(0.0f, 0.0f, 50.0f); // Trace 50 units downwards

    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(HeadCrab); // Ignore self

    bool bHitGround = GetWorld()->LineTraceSingleByChannel(HitResult, HeadCrabStartLocation, HeadCrabEndLocation, ECC_Visibility, Params);
    if (bHitGround==true && bJumping==false)
    {
        return true;
    }
    
    return false;
    
}

bool AHeadCrabAIController::IsRotated(float DeltaTime)
{
     if (HeadCrab==nullptr)
    {
        return false;
    }
    FRotator CurrentRotation = HeadCrab->GetActorRotation();
    FVector DirectionToPlayer = MainCharacter->GetActorLocation() - HeadCrab->GetActorLocation();    
    FRotator NewRotation = FMath::RInterpTo(CurrentRotation, DirectionToPlayer.Rotation(), DeltaTime, 4.0f);
    HeadCrab->SetActorRotation(NewRotation);
    return true;
}

void AHeadCrabAIController::Patrol()
{
     if (HeadCrab==nullptr)
    {
        return;
    }
    HeadCrab->GetCharacterMovement()->MaxWalkSpeed = 100.0f;
    if (HeadCrab->GetPatrolPath()==nullptr)
    {
        return;
    }
    
    PatrolPathPoint = HeadCrab->GetPatrolPath()->GetPatrolPoint(CurrentIndex);
    FVector GlobalPoint = HeadCrab->GetPatrolPath()->GetActorTransform().TransformPosition(PatrolPathPoint);
    MoveToLocation(GlobalPoint);
    // Use a small threshold value to check if the headcrab has reached the patrol point
    float DistanceToTarget = FVector::Dist(HeadCrab->GetActorLocation(), GlobalPoint);
    float AcceptanceRadius = 100.0f; // Adjust as necessary

    if (DistanceToTarget <= AcceptanceRadius)
    {
        if (Forward)
        {
            CurrentIndex++;
        }
        else{CurrentIndex--;}
        if (CurrentIndex>=MaxIndex)
        {
            Forward=false;
        }
        if (CurrentIndex<=0)
        {
            Forward=true;
        }
    }
    
    
}

bool AHeadCrabAIController::CanSeePlayer()
{
    if (IsOnGround())
        {
            if (bCanSee == false)
            {
            return true;
            }
        }
    return false;
}

FString AHeadCrabAIController::GetStateAsString(EHeadcrabState State)
{
    switch (State)
    {
    case EHeadcrabState::Idle:
        return TEXT("Idle");
    case EHeadcrabState::Chasing:
        return TEXT("Chasing");
    case EHeadcrabState::Jumping:
        return TEXT("Jumping");
    case EHeadcrabState::Attacking:
        return TEXT("Attacking");
    case EHeadcrabState::Landing:
        return TEXT("Landing");
    default:
        return TEXT("Unknown");
    }
}

void AHeadCrabAIController::UpdateSightConfig(float NewPeripheralVisionAngle)
{
    if (SightConfig)
    {
        SightConfig->PeripheralVisionAngleDegrees = NewPeripheralVisionAngle;
        PerceptionComponent->RequestStimuliListenerUpdate(); // Force update perception
    }
}


