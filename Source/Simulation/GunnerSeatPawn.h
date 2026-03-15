// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "GunnerSeatPawn.generated.h"

UCLASS()
class SIMULATION_API AGunnerSeatPawn : public APawn
{
    GENERATED_BODY()

public:
    AGunnerSeatPawn();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual void PawnClientRestart() override;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Tank")
    class AMyWheeledVehiclePawn* ParentTank;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    class UInputMappingContext* DefaultMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    class UInputAction* ThrottleAction; 

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    class UInputAction* BrakeAction;    

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    class UInputAction* SteeringAction; 

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    class UInputAction* LookAction;     

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    class UInputAction* FireAction;     

    // Fonksiyonlar
    void ApplyThrottle(const FInputActionValue& Value);
    void ApplyBrake(const FInputActionValue& Value);
    void ApplySteering(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
    void Fire(const FInputActionValue& Value);

    // SERVER RPC (Sunucuya Ýletiþim)
    UFUNCTION(Server, Reliable, WithValidation)
    void Server_UpdateTurret(float YawDelta, float PitchDelta);

    UFUNCTION(Server, Reliable, WithValidation)
    void Server_Fire();
};