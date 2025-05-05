// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "TurretAIController.h"
#include "CombineTurret.h"

// Sets default values
ACombineTurret::ACombineTurret()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BodyMesh=CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Body Mesh"));
    HeadMesh=CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Head Mesh"));
	SpawnP=CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	HeadPivot=CreateDefaultSubobject<USceneComponent>(TEXT("Pivot Point"));
	BodyPivot=CreateDefaultSubobject<USceneComponent>(TEXT("Body Point"));
	
	BodyPivot->SetupAttachment(GetMesh());
	BodyMesh->SetupAttachment(BodyPivot);
	HeadPivot->SetupAttachment(BodyMesh, FName("HeadSocket"));
    HeadMesh->SetupAttachment(HeadPivot);
	SpawnP->SetupAttachment(HeadMesh);
	
}


// Called when the game starts or when spawned
void ACombineTurret::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACombineTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector MeshOffset = GetMesh()->GetComponentLocation() - GetCapsuleComponent()->GetComponentLocation();
    GetCapsuleComponent()->SetWorldLocation(GetCapsuleComponent()->GetComponentLocation() + MeshOffset);


	FRotator MeshRotation = GetMesh()->GetComponentRotation();
    FRotator CapsuleRotation = FRotator (MeshRotation.Pitch, MeshRotation.Yaw+90.f, MeshRotation.Roll);
	
    // Apply the mesh's rotation to the capsule
    GetCapsuleComponent()->SetWorldRotation(CapsuleRotation);
}




void ACombineTurret::GetActorEyesViewPoint(FVector &OutLocation, FRotator &OutRotation) const
{
	Super::GetActorEyesViewPoint(OutLocation,OutRotation);
	OutRotation = GetActorRotation();
}

void ACombineTurret::DestructionHandler()
{
	//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//this->GetMesh()->SetSimulatePhysics(true);
	ATurretAIController* TurretController = Cast<ATurretAIController>(GetController());
	if (TurretController==nullptr)
	{
		return;
	}
	PrimaryActorTick.bCanEverTick = false;
	TurretController->Destroy();
}
