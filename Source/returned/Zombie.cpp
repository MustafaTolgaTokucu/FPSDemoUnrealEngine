// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "HeadCrab.h"
#include "HeadCrabAIController.h"
#include "Zombie.h"
#include "ZombieAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AZombie::AZombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



	//ZombieBody=CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Zombie Body Mesh"));
	ZombieHead=CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Zombie Head Mesh"));
	ZombieHead->SetupAttachment(GetMesh(), FName("HeadSocket"));


}

void AZombie::DestructionHandler()
{
	if (bIsDead==false)
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetSimulatePhysics(true);
		FVector HeadLocation = ZombieHead->GetComponentLocation();
		FRotator HeadRotation = GetMesh()->GetComponentRotation();

		// Destroy the head mesh
		ZombieHead->DestroyComponent();

		// Spawn the headcrab at the location and rotation of the head mesh
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		HeadCrab = GetWorld()->SpawnActor<AHeadCrab>(HeadCrabSub, HeadLocation, HeadRotation, SpawnParams);
		
	}
	AZombieAIController* ZombieController = Cast<AZombieAIController>(this->GetController());
	if(ZombieController)
	{
		ZombieController->Destroy();

	}
	
	
	bIsDead=true;
	
}

// Called when the game starts or when spawned
void AZombie::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//UE_LOG(LogTemp, Warning, TEXT(" to blind %s"), bIsDead? TEXT("true"): TEXT("false"));
}

void AZombie::GetActorEyesViewPoint(FVector &OutLocation, FRotator &OutRotation) const
{
	Super::GetActorEyesViewPoint(OutLocation,OutRotation);
	OutRotation = GetActorRotation();
}

APatrolPath *AZombie::GetPatrolPath() const
{
    return PatrolPath;
}
