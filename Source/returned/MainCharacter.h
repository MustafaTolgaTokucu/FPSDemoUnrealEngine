// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "Components/InputComponent.h"
#include "WeaponsBase.h"
#include "Glock.h"
#include "Shotgun.h"
#include "MainCharacter.generated.h"


class UInputMappingContext;
class UInputAction;

UCLASS()
class RETURNED_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();
	void DestructionHandler();
    APlayerController* GetMainPlayerController() const {return GameController;}
	UPROPERTY(BlueprintReadWrite)
	bool HasBar=false;
	UPROPERTY(BlueprintReadWrite)
	bool BarAttack=false;
	UPROPERTY(BlueprintReadWrite)
	bool HasGlock=false;
	UPROPERTY(BlueprintReadWrite)
	bool GlockAttack=false;
	UPROPERTY(BlueprintReadWrite)
	bool SwapIdle=false;
	UPROPERTY(BlueprintReadWrite)
	bool HasShotgun=false;
	UPROPERTY(BlueprintReadWrite)
	bool bInspect=false;
	/*UPROPERTY(BlueprintReadWrite)
	bool ShotgunAttack=false;*/

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	
	 //cppde kullanmak için mapping context pointerı
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= Girdi)
    UInputMappingContext* OyunIciMappingContext;

	//InputAction blueprinti seçmek için
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= Girdi)
	UInputAction* InputAction;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= Girdi)
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= Girdi)
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= Girdi)
	UInputAction* GrabAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= Girdi)
	UInputAction* PrimaryFireAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= Girdi)
	UInputAction* SwitchWeaponAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= Girdi)
	UInputAction* GlockFireAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= Girdi)
	UInputAction* ReloadAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= Girdi)
	UInputAction* InspectAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= Girdi)
	UInputAction* ToggleFlashligtAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= Girdi)
	UInputAction* DuckAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= Girdi)
	UInputAction* SprintAction;
	/*UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= Girdi)
	UInputAction* SwitchGlock;*/
	
	
	//Ayarlanabilir hareket modifierları
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Movement Modifications")
	FVector JumpHeight;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Movement Modifications")   
    float LineDistance= 400;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Movement Modifications")   
	FVector GrabDist;
	



	//Fonksiyonlar
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jumper(const FInputActionValue& Value);
    void Grab(const FInputActionValue& Value);
    void Grabbed(float DeltaTime); 
	void PrimaryFire(const FInputActionValue& Value);
	void SwitchWeapon(const FInputActionValue& Value);
	void MainFire(const FInputActionValue& Value);
	void Reload(const FInputActionValue& Value);
	void HasWeapon();
	void Inspect(const FInputActionValue& Value);
	void ToggleFlashlight(const FInputActionValue& Value);
	void Duck(const FInputActionValue& Value);
	void Sprint(const FInputActionValue& Value);
	void StopSprinting(const FInputActionValue& Value);
	void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust);
	void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust);
	void UnDuck(const FInputActionValue& Value);
	
	UPROPERTY(BlueprintReadOnly, Category= "Reference")
	AWeaponsBase* Weapon;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool Crowbar = false;
	bool Glock = false;
	bool Shotgun= false;
	UPROPERTY(EditAnywhere)
	class USceneComponent* JumpLocation;
	UPROPERTY(EditAnywhere)
	class UCameraComponent* CameraComponent;
	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArm;
private:
	//Karakter componentları
	UPROPERTY(VisibleAnywhere)
	class UPhysicsHandleComponent* PhysicalHandle;
	
	
	APlayerController* GameController;
	UPROPERTY(EditAnywhere)
	class USpotLightComponent* Flashlight;
	UPROPERTY(EditDefaultsOnly, Category= "Weapons")
	TSubclassOf<ACrowbar> CrowbarBP;
	UPROPERTY(EditDefaultsOnly, Category= "Weapons")
	TSubclassOf<AGlock> GlockBP;
	UPROPERTY(EditDefaultsOnly, Category= "Weapons")
	TSubclassOf<AShotgun> ShotgunBP;
	TArray<AWeaponsBase*> Inventory;
	class UAIPerceptionStimuliSourceComponent* StimulusSource;
	void SetupStimulusSource();


	bool bFlashlightOn;
	bool bDucking;
	
	float DefaultWalkSpeed;
};
