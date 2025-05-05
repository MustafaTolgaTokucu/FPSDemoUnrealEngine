// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerBoxComponente.h"
//#include "Engine/World.h"


UTriggerBoxComponente::UTriggerBoxComponente()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = true;
    
    //Tetikleyici=CreateDefaultSubobject<UHareketlendirici>(TEXT("Kutudan Hareketlendiriciye Erişim"));

}

void UTriggerBoxComponente::BeginPlay()
{
    Super::BeginPlay();

}

void UTriggerBoxComponente::SetMover(UHareketlendirici *NewTetikleyici)
{
    Tetikleyici=NewTetikleyici;
}

void UTriggerBoxComponente::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    InteractorFound();
}





AActor *UTriggerBoxComponente::InteractorFound() const
{
    TArray<AActor*> Actors;
    GetOverlappingActors(Actors);

    AActor* FoundInteractor = nullptr;

    for (AActor* Actor : Actors)
    {
        if (Actor->ActorHasTag(TriggerTag))
        {
            FoundInteractor = Actor;
            break;
        }
    }

    if (FoundInteractor)
    {
        FString BulunanActor = FoundInteractor->GetActorNameOrLabel();
        //UE_LOG(LogTemp, Display, TEXT("Overlapping %s"), *BulunanActor);
        Tetikleyici->MoveActor(true);
        
    }
    else
    {
        Tetikleyici->MoveActor(false);
        //UE_LOG(LogTemp, Warning, TEXT("No Interactor found"));
    }

    return FoundInteractor;

    /*
    Break kullanmadan yapınca belli bir süre sonra hep true kalıyor. ayrıca true olduğu halde no interaction veriyor.
    TArray<AActor*> Actors;
    GetOverlappingActors(Actors);
    for (AActor* Actor:Actors)
    {
    if (Actor->ActorHasTag(TriggerTag))
    { 
        
        FString BulunanActor = Actor->GetActorNameOrLabel();
        UE_LOG(LogTemp, Display, TEXT("overlapping %s"),*BulunanActor);
    if (Tetikleyici)
    {
        Tetikleyici->MoveActor(true);
    }
    
        
        
        return Actor;
    }
    else
    {
    if(Tetikleyici)
    {
        Tetikleyici->MoveActor(false);
        UE_LOG(LogTemp, Warning, TEXT("No Interactor found"));   
    } 
    }
    }
    return nullptr;
}*/
}