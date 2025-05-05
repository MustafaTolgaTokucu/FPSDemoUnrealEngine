// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Hareketlendirici.h"
#include "TriggerBoxComponente.generated.h"


UCLASS(ClassGroup= (Custom), meta=(BlueprintSpawnableComponent))
class RETURNED_API UTriggerBoxComponente : public UBoxComponent
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	UTriggerBoxComponente();
    void SetMover(UHareketlendirici* Tetikleyici);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    
	
    
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private: 
UPROPERTY(EditAnywhere, Category ="Trigger Properties")
FName TriggerTag;
AActor* InteractorFound() const;
UHareketlendirici* Tetikleyici;
	
};
