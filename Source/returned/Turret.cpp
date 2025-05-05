// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "Projectile.h"
// Sets default values
ATurret::ATurret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    BaseMesh=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
    BodyMesh=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body Mesh"));
    HeadMesh=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Head Mesh"));
    Capsule=CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
    SpawnP=CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
    Character=CreateDefaultSubobject<AMainCharacter>(TEXT("Main Character"));
    //RootComponent=Capsule;
    
    BaseMesh->SetupAttachment(Capsule);
    BodyMesh->SetupAttachment(BaseMesh);
    HeadMesh->SetupAttachment(BodyMesh);
    SpawnP->SetupAttachment(HeadMesh);
    Character = nullptr;
    SecondsCounter = 0.0f;
}

void ATurret::DestructionHandler()
{
    //TODO: Visual/sound effects upon death of the turret
    UE_LOG(LogTemp, Warning, TEXT("TURRET DESTROYED"));
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();
	
    // Check if the player controller is valid
     StartTime = GetWorld()->GetTimeSeconds();
        // Get a reference to the player-controlled character
    Character =Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(this,0));
	//GetWorldTimerManager().SetTimer(Zamanlayici,this,&ATurret::TargetDetected,5.f,true);
    
}




// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    TargetDetected();
    
}


void ATurret::TargetDetected()
{   
	
    if (Character)
    {
      
        FVector StartLocation = GetActorLocation();
        FVector EndLocation = Character->GetActorLocation();
        FVector DirectionToPlayer = Character->GetActorLocation() - GetActorLocation();    
        FRotator TurretRot = GetActorRotation();
        FRotator BodyRot = BodyMesh->GetComponentRotation();       
        float RotVal=DirectionToPlayer.Rotation().Yaw;
        //float RotValZ=DirectionToPlayer.Rotation().Roll;
        //float RotValP=DirectionToPlayer.Rotation().Pitch;
        FRotator NewRotation = FRotator(BodyRot.Pitch,RotVal,BodyRot.Roll);
        FRotator FarkRot = NewRotation-TurretRot;     
        if (!IsLineBlocked(StartLocation, EndLocation) && FVector::Dist(StartLocation, EndLocation) < 800.f && (FarkRot.Yaw <60.f && FarkRot.Yaw >- 60.f))
        {
            //BodyMesh->SetWorldRotation(DirectionToPlayer.Rotation()); 
           // SetFocus(Character);
            float CurrentTime = GetWorld()->GetTimeSeconds();
            SecondsCounter = CurrentTime - StartTime;       
            float FireTime=CurrentTime - LastFireTime; 
            if(SecondsCounter>1.5f&&FireTime > FireRate)
            {
            auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass,SpawnP->GetComponentLocation(),SpawnP->GetComponentRotation());
            Projectile->SetOwner(this);
            LastFireTime = CurrentTime;
            
           }
        }
        else
        {
           //BodyMesh->SetWorldRotation(TurretRot);  
           SecondsCounter=0.f;
           StartTime = GetWorld()->GetTimeSeconds();
        }
    }    
}

bool ATurret::IsLineBlocked(const FVector &Start, const FVector &End, AActor *IgnoreActor)
{
	FHitResult HitResult;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(IgnoreActor);
    bool CollisionFound = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_GameTraceChannel4, CollisionParams);
    if(HitResult.GetActor()!=nullptr){
    
    
    //UE_LOG(LogTemp, Warning, TEXT("%s"),*HitResult.GetActor()->GetActorNameOrLabel());
    } 
    // Check if there is any blocking hit between the start and end points
    return CollisionFound;

    
}
