// Fill out your copyright notice in the Description page of Project Settings.


#include "Shotgun.h"
#include "DrawDebugHelpers.h"
#include "MainCharacter.h"

AShotgun::AShotgun()
{
    WeaponName= "Shotgun";
    MagSize = 6.f;
    MaxBulletCarry = 42.f;
    CurrentMagSize = 6.f;
    BulletsCarry = 36.f;
    ReloadTime=0.1;
    NumPellets = 6;
    FireRate=1.1f;
    bReloadingShotgun = false;
    bStartReloadLoop=false;
}

void AShotgun::Attack()
{
    Super::Attack();
    if (Character/*&&Reloading==false*/&&CurrentMagSize>0.f&&bCanAttack==true)
    {    
        if (Reloading)
        {
            Reloading=false;
            bReloadingShotgun = false;
            bStartReloadLoop = false;
            GetWorld()->GetTimerManager().ClearTimer(ReloadHandle);
            
        }
        CanReload=false;
        //Reloading=false;
        ShotgunAttack=true;
        CurrentMagSize-=1.f;
       
        
        // Loop through each pellet
        for (int i = 0; i < NumPellets; ++i)
        {
            RandomDirection = FMath::VRandCone(ForwardVector, SpreadAngle);
            End = Start + (RandomDirection * Range);
            FHitResult HitResult;
            FCollisionQueryParams CollisionParams;
            CollisionParams.AddIgnoredActor(this); // Ignore the player
            bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_GameTraceChannel3, CollisionParams);
            
            //DrawDebugLine(GetWorld(), Start, End, FColor::Red, true);
            if (bHit)
            {
                // Apply damage
                AActor* HitActor = HitResult.GetActor();
                if (HitActor)
                {
                    auto DamageType = UDamageType::StaticClass();
                    DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 3.f, 12, FColor::Red, false, 5.f);   
                    UGameplayStatics::ApplyPointDamage(HitActor, MainCharacterDamage, RandomDirection, HitResult, GetOwner()->GetInstigatorController(), this, DamageType);
                    UE_LOG(LogTemp, Warning, TEXT("%s"),*HitActor->GetActorNameOrLabel());
                    //UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Muzzle);
                    //UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, HitResult.ImpactPoint);
                }
            }
            UE_LOG(LogTemp, Warning, TEXT("%f"), CurrentMagSize);
        }
        bCanAttack = false;
                
        GetWorldTimerManager().SetTimer(FireRateHandle, [this]()
        {
        bCanAttack = true;
        }, FireRate, false);
        if (CurrentMagSize ==0.f)
        {
            
            //Reloading=true;
            GetWorldTimerManager().SetTimer(FireRateHandle, [this]()
            {
            bCanAttack=true;
            Reload();
            GetWorld()->GetTimerManager().ClearTimer(FireRateHandle);
            }, FireRate, false);
        }
            return;
            
        
    }    

} 

                    


float AShotgun::Reload()
{
    Super::Reload();   
    if (CanReload)
    {
        UE_LOG(LogTemp, Warning, TEXT("Reloading Shotgun"));
        if (CurrentMagSize==MagSize||BulletsCarry==0.f)
        {
            Reloading=false;
            UE_LOG(LogTemp, Warning, TEXT("Mag full"));
            GetWorld()->GetTimerManager().ClearTimer(ReloadHandle);
            return CurrentMagSize,BulletsCarry;
        }
        Reloading=true;
       

        GetWorldTimerManager().SetTimer(ReloadHandle, [this]()
        {
            if (bReloadingShotgun)
            {
                BulletsCarry--;
                CurrentMagSize++;
                bReloadingShotgun = false;
            }
            
            if (CurrentMagSize==MagSize||BulletsCarry==0.f)
                {
                    Reloading=false;
                    bReloadingShotgun = false;
                    bStartReloadLoop = false;
                    UE_LOG(LogTemp, Warning, TEXT("Mag full"));
                    bShotgunToIdle=true;
                    GetWorld()->GetTimerManager().ClearTimer(ReloadHandle);
                    return CurrentMagSize,BulletsCarry;
                }  
            return CurrentMagSize,BulletsCarry;                                        
        }, ReloadTime, true);
        
                                            
    }
    
    
    return CurrentMagSize,BulletsCarry;
}
