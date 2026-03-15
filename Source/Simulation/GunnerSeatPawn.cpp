// Fill out your copyright notice in the Description page of Project Settings.

#include "GunnerSeatPawn.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MyWheeledVehiclePawn.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/SpringArmComponent.h"

AGunnerSeatPawn::AGunnerSeatPawn()
{
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;

    USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    RootComponent = SceneRoot;
}

void AGunnerSeatPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AGunnerSeatPawn, ParentTank); // Tank referansýný tüm aða duyur
}

void AGunnerSeatPawn::PawnClientRestart()
{
    Super::PawnClientRestart();

    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            if (DefaultMappingContext)
            {
                Subsystem->AddMappingContext(DefaultMappingContext, 0);
            }
        }
    }
}

void AGunnerSeatPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (ThrottleAction)
        {
            EnhancedInputComponent->BindAction(ThrottleAction, ETriggerEvent::Triggered, this, &AGunnerSeatPawn::ApplyThrottle);
        }
        if (BrakeAction)
        {
            EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Triggered, this, &AGunnerSeatPawn::ApplyBrake);
        }
        if (SteeringAction)
        {
            EnhancedInputComponent->BindAction(SteeringAction, ETriggerEvent::Triggered, this, &AGunnerSeatPawn::ApplySteering);
        }
        if (LookAction)
        {
            EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGunnerSeatPawn::Look);
        }
        if (FireAction)
        {
            EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &AGunnerSeatPawn::Fire);
        }
    }
}


// ÝSTEMCÝ (CLIENT) GÝRDÝ FONKSÝYONLARI
void AGunnerSeatPawn::ApplyThrottle(const FInputActionValue& Value)
{
    float InputValue = Value.Get<float>();
    Server_UpdateTurret(0.0f, InputValue); 
}

void AGunnerSeatPawn::ApplyBrake(const FInputActionValue& Value)
{
    float InputValue = Value.Get<float>();
    Server_UpdateTurret(0.0f, -InputValue); 
}

void AGunnerSeatPawn::ApplySteering(const FInputActionValue& Value)
{
    float InputValue = Value.Get<float>();
    Server_UpdateTurret(InputValue, 0.0f); 
}

void AGunnerSeatPawn::Look(const FInputActionValue& Value)
{
    FVector2D LookAxisVector = Value.Get<FVector2D>();

    AddControllerYawInput(LookAxisVector.X);
    AddControllerPitchInput(LookAxisVector.Y);
}

void AGunnerSeatPawn::Fire(const FInputActionValue& Value)
{
    Server_Fire(); // Ateþ emrini sunucuya ilet
}

// SERVER RPC (Ana Tanktaki Deðiþkenleri Güncelleme)

void AGunnerSeatPawn::Server_UpdateTurret_Implementation(float YawDelta, float PitchDelta)
{
    if (ParentTank)
    {
        const float TurretRotationSpeed = 1.5f;
        const float BarrelRotationSpeed = 1.0f;

        ParentTank->TurretYaw = FMath::Fmod(ParentTank->TurretYaw + (YawDelta * TurretRotationSpeed), 360.0f);
        ParentTank->BarrelPitch = FMath::Clamp(ParentTank->BarrelPitch + (PitchDelta * BarrelRotationSpeed), -10.0f, 45.0f);
    }
}

bool AGunnerSeatPawn::Server_UpdateTurret_Validate(float YawDelta, float PitchDelta)
{
    return true;
}

void AGunnerSeatPawn::Server_Fire_Implementation()
{
    if (ParentTank)
    {
        ParentTank->Server_Fire_Implementation();
    }
}

bool AGunnerSeatPawn::Server_Fire_Validate()
{
    return true;
}