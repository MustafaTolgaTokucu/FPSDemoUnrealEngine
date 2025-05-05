// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PatrolPath.h"
#include "Zombie.generated.h"

UCLASS()
class RETURNED_API AZombie : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AZombie();
	void DestructionHandler();
	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* ZombieHead;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(BlueprintReadOnly, Category= "Reference")
	class AHeadCrab* HeadCrab;
	class AHeadCrabAIController* HeadCrabController;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	
    virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;
	APatrolPath* GetPatrolPath() const;
private: 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(AllowPrivateAccess="true"))
	APatrolPath* PatrolPath;
	UPROPERTY(EditDefaultsOnly, Category= "HeadCrab")
	class TSubclassOf<AHeadCrab> HeadCrabSub;
	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	bool bIsDead=false;
};
