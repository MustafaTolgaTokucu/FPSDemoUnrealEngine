// Fill out your copyright notice in the Description page of Project Settings.


#include "Glock.h"
#include "DrawDebugHelpers.h"
#include "MainCharacter.h"

AGlock::AGlock()
{
    MagSize = 18.f;
    MaxBulletCarry = 216.f;
    CurrentMagSize = 18.f;
    BulletsCarry = 150.f;
    WeaponName="Glock";
    ReloadTime=1.5f;
}

void AGlock::Attack()
{
    Super::Attack();
    // Set max range
    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);
    if (Character&&Reloading==false)
    {
        Character->GlockAttack=true;
        bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_GameTraceChannel3, Params);
        //DrawDebugLine(GetWorld(), Start, End, FColor::Red, true);
        CurrentMagSize-=1.f;
    }
    if (bHit)
    {
        DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 3.f, 12, FColor::Red, false, 5.f);
        // Apply damage
        AActor* HitActor = HitResult.GetActor();
        if (HitActor)
        {
        
        auto DamageType = UDamageType::StaticClass();
            
        UGameplayStatics::ApplyPointDamage(HitActor, MainCharacterDamage, ForwardVector, HitResult, GetOwner()->GetInstigatorController(), this, DamageType);
        UE_LOG(LogTemp, Warning, TEXT("%s"),*HitActor->GetActorNameOrLabel());
        UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Muzzle);
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, HitResult.ImpactPoint);
        }
    }
    if (CurrentMagSize ==0.f)
    {
        
        Reload();
        return;
    }
}

void AGlock::CombineAttack()
{
    Super::CombineAttack();
    SpreadDegree=5.f;
    RandomDirection = FMath::VRandCone(CombineForwardVector, SpreadAngle);
    CombineEnd = CombineStart + (RandomDirection * Range);
    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);
    
    if (Combine)
    {
        //DrawDebugLine(GetWorld(), CombineStart, CombineEnd, FColor::Red, true);
        bHit = GetWorld()->LineTraceSingleByChannel(HitResult, CombineStart, CombineEnd, ECC_GameTraceChannel4, Params);
    }
    if (bHit)
    {
        // Apply damage
        AActor* HitActor = HitResult.GetActor();
        if (HitActor)
        {
        
        auto DamageType = UDamageType::StaticClass();
        UGameplayStatics::ApplyPointDamage(HitActor, CombineDamage, RandomDirection, HitResult, GetInstigatorController(), this, DamageType);
        UE_LOG(LogTemp, Warning, TEXT("%s"),*HitActor->GetActorNameOrLabel());
        UE_LOG(LogTemp, Warning, TEXT("Combine hit"));
        UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Muzzle);
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, HitResult.ImpactPoint);
        }
    }
}

float AGlock::Reload()
{
    Super::Reload();
    return CurrentMagSize,BulletsCarry;
}
