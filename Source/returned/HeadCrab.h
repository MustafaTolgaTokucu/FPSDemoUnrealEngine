// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PatrolPath.h"
#include "HeadCrabAIController.h"

#include "HeadCrab.generated.h"




UCLASS()
class RETURNED_API AHeadCrab : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHeadCrab();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void DestructionHandler();

	UPROPERTY(BlueprintReadwrite, Category= "Reference")
	AHeadCrabAIController* HeadCrabController;
	class AMainCharacter* MainCharacter;
	APatrolPath* GetPatrolPath() const;
	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;
	
private: 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(AllowPrivateAccess="true"))
	APatrolPath* PatrolPath;
	
	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	bool bDead;
	
	
};
