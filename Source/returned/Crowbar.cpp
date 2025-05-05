// Fill out your copyright notice in the Description page of Project Settings.


#include "Crowbar.h"
#include "DrawDebugHelpers.h"
#include "MainCharacter.h"

ACrowbar::ACrowbar()
{
    WeaponName="Crowbar";
    bCanAttack=true;
    FireRate= 0.3f;
    HasHit=false;
}


void ACrowbar::Attack()
{
    
    Super::Attack();
    // Set max range
    if (bCanAttack)
    {
    End= Start+(ForwardVector*175.f);
    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);
    
    //DrawDebugLine(GetWorld(), Start, End, FColor::Red, true);
    bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_GameTraceChannel3, Params);
    if (bHit)
    {
        // Apply damage
        AActor* HitActor = HitResult.GetActor();
        if (HitActor)
        {
            DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 3.f, 12, FColor::Red, false, 5.f);
            HasHit=true;
            Character->BarAttack=false;
            auto DamageType = UDamageType::StaticClass();
            
                UGameplayStatics::ApplyPointDamage(HitActor, MainCharacterDamage, ForwardVector, HitResult, GetInstigatorController(), this, DamageType);
                UE_LOG(LogTemp, Warning, TEXT("%s"),*HitActor->GetActorNameOrLabel());
                bCanAttack = false;
                GetWorldTimerManager().SetTimer(FireRateHandle, [this]()
                {
                bCanAttack = true;
                }, FireRate, false);
                //TODO FX
        }
    }
    else
            {
                //bCanAttack = false;
                /* GetWorldTimerManager().SetTimer(FireRateHandle, [this]()
                        {
                        bCanAttack = true;
                        }, FireRate, false);
                HasHit=false;*/
                Character->BarAttack=true;
            }

    }
    
    
}
