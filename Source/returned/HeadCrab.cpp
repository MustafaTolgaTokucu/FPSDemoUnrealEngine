// Fill out your copyright notice in the Description page of Project Settings.


#include "HeadCrab.h"
#include "MainCharacter.h"
//#include "AIController.h"
#include "Components/ArrowComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AHeadCrab::AHeadCrab()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AHeadCrab::OnMeshHit);
	bDead=false;
}

// Called when the game starts or when spawned
void AHeadCrab::BeginPlay()
{
	Super::BeginPlay();
	HeadCrabController = Cast<AHeadCrabAIController> (GetController());
	MainCharacter =Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(this,0));
	//
	
	
}

// Called every frame
void AHeadCrab::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
   
    

}



void AHeadCrab::DestructionHandler()
{
	bDead = true;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	this->GetMesh()->SetSimulatePhysics(true);
	if (HeadCrabController==nullptr)
	{
		return;
	}
	
	HeadCrabController->Destroy();
}

APatrolPath *AHeadCrab::GetPatrolPath() const
{
    return PatrolPath;
}

void AHeadCrab::GetActorEyesViewPoint(FVector &OutLocation, FRotator &OutRotation) const
{
	Super::GetActorEyesViewPoint(OutLocation,OutRotation);
	OutRotation = GetActorRotation();
}

