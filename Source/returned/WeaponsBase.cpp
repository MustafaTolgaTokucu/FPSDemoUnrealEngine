// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponsBase.h"
#include "MainCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Combine.h"
// Sets default values
AWeaponsBase::AWeaponsBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh=CreateDefaultSubobject<USkeletalMeshComponent>("Skeletal Mesh");
	Root=CreateDefaultSubobject<USceneComponent>("Root Component");
	SetRootComponent(Root);
	Mesh->SetupAttachment(Root);
	Muzzle=CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle"));
    Muzzle->SetupAttachment(Mesh);
	SpreadDegree=8.f;
	SpreadAngle = FMath::DegreesToRadians(SpreadDegree);
	Range = 10000.0f;
	Reloading=false;
}

// Called when the game starts or when spawned
void AWeaponsBase::BeginPlay()
{
	Super::BeginPlay();
	Character =Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(this,0));
	
}

// Called every frame
void AWeaponsBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//Reloading=false;
	//UE_LOG(LogTemp, Display, TEXT("reload %s"),bReloadingShotgun ? TEXT("true"):TEXT("false"));
	
}

void AWeaponsBase::Attack()
{
	
	if (Character)
    {
    Start = Character->CameraComponent->GetComponentLocation();
    ForwardVector = Character->CameraComponent->GetForwardVector();
	End = Start + (ForwardVector * Range);
	//CurrentMagSize-=1.f;
    }
	
}
void AWeaponsBase::CombineAttack()
{
	Combine = Cast<ACombine>(WeaponOwner);
	if(Combine)
	{
	CombineStart = Combine->GetActorLocation();
    //CombineForwardVector = Combine->GetArrowComponent()->GetForwardVector();
	CombineForwardVector = Combine->GetActorForwardVector()-Character->GetActorForwardVector();
	//CombineEnd = CombineStart + (CombineForwardVector * 10000.0f);
	CombineEnd = Character->GetActorLocation();
	UE_LOG(LogTemp, Error, TEXT("combine alert"));
	}
}

float AWeaponsBase::Reload()
{	
	if (WeaponName!="Shotgun")
	{
		
		if (MagSize==CurrentMagSize)
		{
			return CurrentMagSize,BulletsCarry;
		}
		
		if (BulletsCarry>0.f&&CurrentMagSize!=MagSize)
		{
			Reloading=true;
			RequiredBullet=MagSize-CurrentMagSize;
			if (RequiredBullet>BulletsCarry)
			{
				
				GetWorldTimerManager().SetTimer(FireRateHandle, [this]()
					{
					CurrentMagSize+=BulletsCarry;
					BulletsCarry=0.f;
					//Reloading=false;
					}, ReloadTime, false);
			}
			else
			{
				GetWorldTimerManager().SetTimer(FireRateHandle, [this]()
					{
					BulletsCarry-=RequiredBullet;
					CurrentMagSize=MagSize;
					//Reloading=false;
					}, ReloadTime, false);
			
			}
		}
		else{Reloading=false;}
		
	}
	return CurrentMagSize,BulletsCarry;
	
}


