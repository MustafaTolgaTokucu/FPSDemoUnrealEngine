// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CombineAIController.h"
//#include "AIController.h"
#include "Glock.h"
#include "PatrolPath.h"
#include "Combine.generated.h"


UCLASS()
class RETURNED_API ACombine : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACombine();
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* BodyMesh;

	UPROPERTY(BlueprintReadwrite, Category= "Reference")
	ACombineAIController* CombineAiController;
	UPROPERTY(BlueprintReadwrite)
	bool bCombineFire;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void DestructionHandler();
	APatrolPath* GetPatrolPath() const;
	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, Category= "Weapons")
	TSubclassOf<AGlock> GlockBP;
	class AWeaponsBase* Weapon;
	void Attack();
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(AllowPrivateAccess="true"))
	APatrolPath* PatrolPath;
	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	bool bDead;
};
