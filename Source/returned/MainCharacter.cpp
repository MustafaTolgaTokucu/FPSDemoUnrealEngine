// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"

//#include "DrawDebugHelpers.h"
#include "Components/SceneComponent.h"
#include "Math/Vector.h"
#include "Engine/World.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SpotLightComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
//#include "Components/SkeletalMeshComponent.h"
#include "Crowbar.h"
#include "Glock.h"
#include "Shotgun.h"
#include "TimerManager.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
   
    PhysicalHandle=CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("Physics Handle Component"));
	CameraComponent=CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	Flashlight = CreateDefaultSubobject<USpotLightComponent>(TEXT("Flashlight"));
	JumpLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Jump Location"));
	SpringArm->SetupAttachment(RootComponent);
	CameraComponent->SetupAttachment(SpringArm);
	Flashlight->SetupAttachment(GetMesh());
	bFlashlightOn= false;
	JumpLocation->SetupAttachment(GetMesh());
	BarAttack=false;
	SetupStimulusSource();
	bDucking = false;
}

void AMainCharacter::DestructionHandler()
{
	
    SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	this->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel4, ECR_Ignore);
	Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	Weapon->Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Weapon->Mesh->SetSimulatePhysics(true);
	if (StimulusSource)
    {
        StimulusSource->UnregisterFromPerceptionSystem();
    }
	//UE_LOG(LogTemp, Warning, TEXT("YOU ARE DEAD"));
	
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed=350.f;
	DefaultWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	
	GameController = Cast<APlayerController>(GetController());
	//For Crowbar
	SwapIdle=true;
	Inventory.Add(GetWorld()->SpawnActor<ACrowbar>(CrowbarBP));
	Weapon=Inventory[0];
	Weapon->AttachToComponent(GetMesh(),FAttachmentTransformRules::KeepRelativeTransform,TEXT("WeaponSocket"));
    Inventory.Add(GetWorld()->SpawnActor<AGlock>(GlockBP));
	Inventory.Add(GetWorld()->SpawnActor<AShotgun>(ShotgunBP));
	//Does player have it currently?
	Crowbar=true;
	Glock=true;
	Shotgun = true;

	
    if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GameController->GetLocalPlayer()))
    {
		Subsystem->AddMappingContext(OyunIciMappingContext, 0);
	}

}
// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    Grabbed(DeltaTime);
	HasWeapon();
}
// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    if(UEnhancedInputComponent* EnhancedInputComponent=CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &AMainCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMainCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMainCharacter::Jumper);
		EnhancedInputComponent->BindAction(GrabAction, ETriggerEvent::Triggered, this, &AMainCharacter::Grab);
		EnhancedInputComponent->BindAction(PrimaryFireAction, ETriggerEvent::Triggered, this, &AMainCharacter::PrimaryFire);
		EnhancedInputComponent->BindAction(SwitchWeaponAction, ETriggerEvent::Triggered, this, &AMainCharacter::SwitchWeapon);
		EnhancedInputComponent->BindAction(GlockFireAction, ETriggerEvent::Triggered, this, &AMainCharacter::MainFire);
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &AMainCharacter::Reload);
		EnhancedInputComponent->BindAction(InspectAction, ETriggerEvent::Triggered, this, &AMainCharacter::Inspect);
		EnhancedInputComponent->BindAction(ToggleFlashligtAction, ETriggerEvent::Triggered, this, &AMainCharacter::ToggleFlashlight);
		EnhancedInputComponent->BindAction(DuckAction, ETriggerEvent::Started, this, &AMainCharacter::Duck);
		EnhancedInputComponent->BindAction(DuckAction, ETriggerEvent::Completed, this, &AMainCharacter::UnDuck);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AMainCharacter::Sprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AMainCharacter::StopSprinting);
	}
}

void AMainCharacter::SetupStimulusSource()
{
	StimulusSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));
	if (StimulusSource)
	{
		StimulusSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
		StimulusSource->RegisterWithPerceptionSystem();
	}
	
}

void AMainCharacter::Move(const FInputActionValue& Value)
{
FVector2D MoveValue = Value.Get<FVector2D>();
    FVector Forward = GetActorForwardVector();
    FVector Right = GetActorRightVector();
    
	Forward.Z = 0;
	Forward.Normalize();
    // Normalize the move input to avoid faster diagonal movement
    
    //Normalize the move input to avoid faster diagonal movement
    if (!MoveValue.IsNearlyZero())
    {
        MoveValue.Normalize();
    }
    // Calculate the movement direction in the XY-plane
    FVector MovementDirection = Forward * MoveValue.X + Right * MoveValue.Y;
    //UE_LOG(LogTemp, Warning, TEXT("aktör direction vektörü%s"), *Forward.ToString());
	
    // Apply movement input to the character
    AddMovementInput(MovementDirection );
	
}

void AMainCharacter::Look(const FInputActionValue &Value)
{
	const FVector2D LookValue = Value.Get<FVector2D>();
	if(Controller)
	{
    AddControllerYawInput(LookValue.X);
	AddControllerPitchInput(LookValue.Y);

	}
}

void AMainCharacter::Jumper(const FInputActionValue &Value)

{
	bool Jumped = Value.Get<bool>();
	/*if(Jumped){
	LaunchCharacter(JumpHeight, false, true);
	}*/
	Jump();
}

void AMainCharacter::Grab(const FInputActionValue& Value)
{
	bool Touched =Value.Get<bool>();
	//eğer başta nesne tutuyorsa, bıraksın.
	if(PhysicalHandle->GetGrabbedComponent() != nullptr){
		PhysicalHandle->ReleaseComponent();
		return;
	}
	
	//Debug line boyunca çarpan nesneyi algılamak için kullanılır, FQuad identity rotasyon olmadığını gösterir, trace channel bulmak için config dosyasına 
	//...bakılır.
    FVector Start = GetActorLocation();
	FVector End = Start+ GetActorForwardVector()*LineDistance;
    //DrawDebugLine(GetWorld(),Start, End, FColor::Red);
	FHitResult HitResult;
	//DrawDebugSphere(GetWorld(),HitResult.ImpactPoint,10.0f,10.0f,FColor::Blue, false,5 );
	FCollisionShape Sphere = FCollisionShape::MakeSphere(10.0f);
    bool Detect = GetWorld()->SweepSingleByChannel(HitResult,Start,End, FQuat::Identity,ECC_GameTraceChannel2,Sphere);
	UPrimitiveComponent* HitObject = HitResult.GetComponent();
	if(HitObject==nullptr)
	{return;}
	HitObject -> WakeAllRigidBodies();
	PhysicalHandle->GrabComponentAtLocationWithRotation(HitObject,NAME_None,HitObject->GetComponentLocation(),GetActorRotation());
	
    
}

void AMainCharacter::Grabbed(float DeltaTime)
{
PhysicalHandle= FindComponentByClass<UPhysicsHandleComponent>();
    if(PhysicalHandle==nullptr){
		UE_LOG(LogTemp, Warning, TEXT("no phyndl"));
		return;
	}
    UPrimitiveComponent* HoldComponent = PhysicalHandle->GetGrabbedComponent();
    if(HoldComponent==nullptr){return;}
    FVector OLocation = HoldComponent->GetComponentLocation();

    float LetGo = FVector::Distance(GetActorLocation(),OLocation);
	//UE_LOG(LogTemp, Display, TEXT("obje: %f"),LetGo);
	GrabDist= GetActorLocation() + GetActorForwardVector()*100.0f;
	PhysicalHandle->SetTargetLocationAndRotation(GrabDist,GetActorRotation());
	
    if (LetGo > 130.0f) {
    PhysicalHandle->ReleaseComponent();
    UE_LOG(LogTemp, Warning, TEXT("Released"));
    }


}

void AMainCharacter::PrimaryFire(const FInputActionValue &Value)
{
	if (Weapon&&Weapon->WeaponName=="Crowbar")
	{
		//BarAttack=true;
		SwapIdle=true;
		Weapon->Attack();
	}
}

void AMainCharacter::SwitchWeapon(const FInputActionValue& Value)
{
	
	float InputValue = Value.Get<float>(); 
	
	UE_LOG(LogTemp, Warning, TEXT("%f"), InputValue);
	//UE_LOG(LogTemp, Warning, TEXT("float: %f"), InputValue);
	int WeaponIndex = FMath::RoundToInt(InputValue);
	//UE_LOG(LogTemp, Warning, TEXT("%i"),WeaponIndex );
	UE_LOG(LogTemp, Display, TEXT("%i"), WeaponIndex);
	switch (WeaponIndex)
	{
	case 1:
		if (Crowbar&&Weapon->WeaponName!="Crowbar")
		{
			//Weapon->Destroy();	
			SwapIdle=true;
			Weapon->SetActorHiddenInGame(true);;		
			//Inventory.Add(GetWorld()->SpawnActor<ACrowbar>(CrowbarBP));
			//Weapon=Inventory.Last();
			Weapon=Inventory[0];
			Weapon->AttachToComponent(GetMesh(),FAttachmentTransformRules::KeepRelativeTransform,TEXT("WeaponSocket"));
			Weapon->SetOwner(this);
			Weapon->SetActorHiddenInGame(false);

			//UE_LOG(LogTemp, Warning, TEXT("%s"),*Weapon->WeaponName);
			
		}
		break;
	case 2:
	if (Glock&&Weapon->WeaponName!="Glock"&&Inventory.IsValidIndex(1))
	{
		Weapon->SetActorHiddenInGame(true);
		Weapon=Inventory[1];
		Weapon->AttachToComponent(GetMesh(),FAttachmentTransformRules::KeepRelativeTransform,TEXT("WeaponSocket"));
		Weapon->SetActorHiddenInGame(false);
		Weapon->SetOwner(this);
		//UE_LOG(LogTemp, Warning, TEXT("%s"),*Weapon->WeaponName);
	}
		break;	
	case 3:
	UE_LOG(LogTemp, Display, TEXT("shotgn"));
	if (Shotgun&&Weapon->WeaponName!="Shotgun"&&Inventory.IsValidIndex(2))
	{
		Weapon->SetActorHiddenInGame(true);
		Weapon=Inventory[2];
		//Weapon->AttachToComponent(GetMesh(),FAttachmentTransformRules::KeepRelativeTransform,TEXT("WeaponSocket"));
		Weapon->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetIncludingScale,TEXT("WeaponSocket"));
		
		Weapon->SetActorHiddenInGame(false);
		Weapon->SetOwner(this);
		//UE_LOG(LogTemp, Warning, TEXT("%s"),*Weapon->WeaponName);
	}
	break;			
	default:

		break;
	}
}

void AMainCharacter::MainFire(const FInputActionValue &Value)
{

	if (Weapon)
	{
		Weapon->Attack();
	}
	
}

void AMainCharacter::Reload(const FInputActionValue &Value)
{
	if (Weapon)
	{
		Weapon->Reload();
	}
	
}

void AMainCharacter::HasWeapon()
{
	if (Weapon&& Weapon->WeaponName=="Crowbar")
	{
		HasBar=true;
		//BarAttack=false;
		//UE_LOG(LogTemp, Warning, TEXT("%s"),BarAttack ? TEXT("true"):TEXT("false"));
		
	}
	else{HasBar=false;}
	if (Weapon&& Weapon->WeaponName=="Glock")
	{
		HasGlock=true;
		//GlockAttack=false;
		//UE_LOG(LogTemp, Warning, TEXT("%s"),BarAttack ? TEXT("true"):TEXT("false"));
		
	}
	else{HasGlock=false;}
	if (Weapon&& Weapon->WeaponName=="Shotgun")
	{
		HasShotgun=true;
	}
	else{HasShotgun=false;}
}

void AMainCharacter::Inspect(const FInputActionValue &Value)
{
	bInspect=true;
	
}

void AMainCharacter::ToggleFlashlight(const FInputActionValue &Value)
{
	bFlashlightOn = !bFlashlightOn;
	Flashlight->SetVisibility(bFlashlightOn);
}

void AMainCharacter::Duck(const FInputActionValue &Value)
{
	
	//UE_LOG(LogTemp, Display, TEXT("dak,ng"));
	Crouch();
	
	
	
}

void AMainCharacter::Sprint(const FInputActionValue &Value)
{
	//bool bIsSprinting = Value.Get<bool>();
	//float DefaultWalkSpeed =GetCharacterMovement()->MaxWalkSpeed;
    /*if (bIsSprinting)
    {

        
    }*/
    GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed * 2.f;

}

void AMainCharacter::StopSprinting(const FInputActionValue &Value)
{
	
	UE_LOG(LogTemp, Display, TEXT("Sprint over"));
    GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
}
void AMainCharacter::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
    Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);

    // Adjust the mesh location to match the capsule
    FVector MeshLocation = GetMesh()->GetRelativeLocation();;
    MeshLocation.Z -= HalfHeightAdjust;
    GetMesh()->SetRelativeLocation(MeshLocation);
}

void AMainCharacter::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
    Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);

    // Reset the mesh location when standing up
    /*FVector MeshLocation = GetMesh()->GetRelativeLocation();;
    MeshLocation.Z += HalfHeightAdjust;
    GetMesh()->SetRelativeLocation(MeshLocation);*/
}

void AMainCharacter::UnDuck(const FInputActionValue &Value)
{
	UnCrouch();
}
