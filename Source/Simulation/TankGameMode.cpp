// Fill out your copyright notice in the Description page of Project Settings.


#include "TankGameMode.h"
#include "TankController.h"
#include "MyWheeledVehiclePawn.h"
#include "GunnerSeatPawn.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

ATankGameMode::ATankGameMode()
{
    ConnectedPlayers = 0;

    PlayerControllerClass = ATankController::StaticClass();
}

void ATankGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
	ConnectedPlayers++; // Oyuncu girdi, sayacý artýr(Rol geçici feature'ýnda kullanýlacak)
}

void ATankGameMode::Logout(AController* Exiting)
{
    Super::Logout(Exiting);
    ConnectedPlayers--; // Oyuncu çýktý, sayacý azalt(Rol geçici feature'ýnda kullanýlacak)
}

void ATankGameMode::HandleRoleSwitchRequest(APlayerController* RequestingPlayer)
{
    if (ConnectedPlayers > 1)
    {
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("2 Oyuncu Bagli: Rol Degisimi Kilitli!"));
        return;
    }

    if (MainTank && GunnerSeat)
    {
        APawn* CurrentPawn = RequestingPlayer->GetPawn();

        if (CurrentPawn == MainTank)
        {
            RequestingPlayer->Possess(GunnerSeat);
            if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Nisanci Rolune Gecildi!"));
        }
        else if (CurrentPawn == GunnerSeat)
        {
            RequestingPlayer->Possess(MainTank);
            if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Surucu Rolune Gecildi!"));
        }
    }
}

void ATankGameMode::AssignPlayerRole(APlayerController* Player, bool bIsDriver)
{
    if (!MainTank)
    {
        MainTank = Cast<AMyWheeledVehiclePawn>(UGameplayStatics::GetActorOfClass(GetWorld(), AMyWheeledVehiclePawn::StaticClass()));
    }

    if (MainTank && !GunnerSeat)
    {
   
        UClass* SpawnClass = AGunnerSeatPawn::StaticClass();

        if (GunnerSeatClass)
        {
            SpawnClass = GunnerSeatClass;
        }

        GunnerSeat = GetWorld()->SpawnActor<AGunnerSeatPawn>(SpawnClass, MainTank->GetActorLocation(), MainTank->GetActorRotation());
        GunnerSeat->ParentTank = MainTank;

        if (MainTank->TopSkeletalMesh)
        {
            GunnerSeat->AttachToComponent(MainTank->TopSkeletalMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
        }
        else
        {
            GunnerSeat->AttachToActor(MainTank, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
        }
    }

    if (bIsDriver && MainTank)
    {
        Player->Possess(MainTank);
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Oyuncu Surucu Olarak Katildi!"));
    }
    else if (!bIsDriver && GunnerSeat)
    {
        Player->Possess(GunnerSeat);
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Oyuncu Nisanci Olarak Katildi!"));
    }
}