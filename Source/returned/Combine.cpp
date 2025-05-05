// Fill out your copyright notice in the Description page of Project Settings.


#include "Combine.h"
#include "WeaponsBase.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "MainCharacter.h"
// Sets default values
ACombine::ACombine()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BodyMesh=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body Mesh"));
	BodyMesh->SetupAttachment(this->GetMesh());
	bDead=false;
}

// Called when the game starts or when spawned
void ACombine::BeginPlay()
{
	Super::BeginPlay();
	Weapon=GetWorld()->SpawnActor<AGlock>(GlockBP);
	Weapon->AttachToComponent(GetMesh(),FAttachmentTransformRules::KeepRelativeTransform,TEXT("GripPoint"));
	Weapon->WeaponOwner=this;
	CombineAiController = Cast<ACombineAIController> (GetController());
	
	//this->GetCharacterMovement()->MaxWalkSpeed=20;
}



// Called every frame
void ACombine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//UE_LOG(LogTemp, Display, TEXT("speed %f"), this->GetCharacterMovement()->MaxWalkSpeed);
	if(CombineAiController&&CombineAiController->bCanSee)
	{
		CombineAiController->FocusToPlayer();
	}
	
}

void ACombine::DestructionHandler()
{
	//UE_LOG(LogTemp, Warning, TEXT("Enemy Died"));
	
	//DetachFromControllerPendingDestroy();
	bDead=true;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	this->GetMesh()->SetSimulatePhysics(true);
	//Destroy();
	Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	Weapon->Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Weapon->Mesh->SetSimulatePhysics(true);
	if (CombineAiController==nullptr)
	{
		return;
	}
	
	CombineAiController->Destroy();
}


APatrolPath * ACombine::GetPatrolPath() const
{

return PatrolPath;
}


void ACombine::Attack()
{
	UE_LOG(LogTemp, Display, TEXT("attacking"));
	
	Weapon->CombineAttack();
	bCombineFire = true;
}
// Called to bind functionality to input
/*void ACombine::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

}*/
