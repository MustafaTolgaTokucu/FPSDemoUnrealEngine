// Fill out your copyright notice in the Description page of Project Settings.


#include "LinearFPSPlayerController.h"
#include "GameFramework/Pawn.h"

void ALinearFPSPlayerController::SetPlayerEnabledState(bool bPlayerEnabled)
{
    if (bPlayerEnabled)
    {
        GetPawn()->EnableInput(this);
    }
    else{
        GetPawn()->DisableInput(this);
        UE_LOG(LogTemp, Error, TEXT("Input Disabled"));
    }
    //bShowMouseCursor = bPlayerEnabled;
}
