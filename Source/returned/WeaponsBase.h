// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"

#include "WeaponsBase.generated.h"

class ACrowbar;
UCLASS()
class RETURNED_API AWeaponsBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponsBase();
	AActor* WeaponOwner;
	
	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* Mesh;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, Category= "Damage")
	float MainCharacterDamage;
	UPROPERTY(EditAnywhere,Category= "Damage")
	float CombineDamage;
	//Components
	
	class USceneComponent* Root;
	UPROPERTY(EditAnywhere)
	USceneComponent* Muzzle;	
	class AMainCharacter* Character;
	class ACombine* Combine;
	//Reloading Variables
	UPROPERTY(BlueprintReadWrite)
	float CurrentMagSize;
	float MaxBulletCarry;
	float MagSize;
	UPROPERTY(BlueprintReadWrite)
	float BulletsCarry;
	float RequiredBullet;
	UPROPERTY(BlueprintReadWrite)
	bool Reloading;
	UPROPERTY(BlueprintReadWrite)
	bool ShotgunAttack=false;
	UPROPERTY(BlueprintReadWrite)
	bool CanReload=true;
	UPROPERTY(BlueprintReadWrite)
	bool bShotgunToIdle=false;
	float ReloadTime;
	UPROPERTY(BlueprintReadWrite)
	bool bReloadingShotgun;
	UPROPERTY(BlueprintReadWrite)
	bool bStartReloadLoop;

	FVector Start;
	FVector ForwardVector;
	FVector End;
	FVector CombineStart;
	FVector CombineForwardVector;
	FVector CombineEnd;
	bool bHit;
	
	// Timer handle for attack cooldown
    FTimerHandle FireRateHandle;
	FTimerHandle ReloadHandle;
    // Cooldown duration in seconds
    float FireRate;
	bool bCanAttack=true;
	int NumPellets;
	float SpreadAngle;
	float SpreadDegree;
	float Range;
	FVector RandomDirection;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Attack();
	virtual void CombineAttack();
	virtual float Reload();
	
	/*UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACrowbar> CrowbarBP;*/
	
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    FString WeaponName;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	bool HasHit;
	
private:
	
	
};
