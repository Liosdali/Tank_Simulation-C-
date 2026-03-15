// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TankGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SIMULATION_API ATankGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
    ATankGameMode();

    // Bir oyuncu oyuna girdiðinde veya çýktýðýnda tetiklenir
    virtual void PostLogin(APlayerController* NewPlayer) override;
    virtual void Logout(AController* Exiting) override;

    void HandleRoleSwitchRequest(APlayerController* RequestingPlayer);


    UPROPERTY()
    class AMyWheeledVehiclePawn* MainTank;

    UPROPERTY()
    class AGunnerSeatPawn* GunnerSeat;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Classes")
    TSubclassOf<class AGunnerSeatPawn> GunnerSeatClass;

    void AssignPlayerRole(APlayerController* Player, bool bIsDriver);

private:
    // Aktif oyuncu sayýsý
    int32 ConnectedPlayers;
};
