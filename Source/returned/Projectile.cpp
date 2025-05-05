// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
    MermiMesh=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mermi Mesh"));
	RootComponent=MermiMesh;
	PMovement=CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	/*PMovement->InitialSpeed=20;
	PMovement->MaxSpeed=50;*/
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	MermiMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}


// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnHit(UPrimitiveComponent *hitter, AActor *HitActor, UPrimitiveComponent* OtherObject, FVector ImpulseHit, const FHitResult& HitResult)
{

auto MyOwner = GetOwner();
if (MyOwner == nullptr)return;
auto MyOwnerInstigator = MyOwner->GetInstigatorController();
auto DamageTypeClass = UDamageType::StaticClass();
if (HitActor && HitActor != this && HitActor != MyOwner)
{
	UGameplayStatics::ApplyDamage(HitActor, DamageAmount, MyOwnerInstigator, this, DamageTypeClass);
	Destroy();
}




}
