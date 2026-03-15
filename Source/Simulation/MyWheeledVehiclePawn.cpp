// Fill out your copyright notice in the Description page of Project Settings.

#include "MyWheeledVehiclePawn.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

AMyWheeledVehiclePawn::AMyWheeledVehiclePawn()
{
    PrimaryActorTick.bCanEverTick = true;

	// Deðiþkenlerin að üzerinde replike edilmesi için aktif edilmeli
    bReplicates = true;
    SetReplicateMovement(true);

    CurrentRole = EArmorRole::Driver;
}

// Senkronize edilecek deðiþkenler
void AMyWheeledVehiclePawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AMyWheeledVehiclePawn, CurrentRole);
    DOREPLIFETIME(AMyWheeledVehiclePawn, TurretYaw);
    DOREPLIFETIME(AMyWheeledVehiclePawn, BarrelPitch);
}

void AMyWheeledVehiclePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // Gaz
        if (ThrottleAction)
        {
            EnhancedInputComponent->BindAction(ThrottleAction, ETriggerEvent::Triggered, this, &AMyWheeledVehiclePawn::ApplyThrottle);
            EnhancedInputComponent->BindAction(ThrottleAction, ETriggerEvent::Completed, this, &AMyWheeledVehiclePawn::ApplyThrottle);
        }

        // Fren
        if (BrakeAction)
        {
            EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Triggered, this, &AMyWheeledVehiclePawn::ApplyBrake);
            EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Completed, this, &AMyWheeledVehiclePawn::ApplyBrake);
        }

        // Direksiyon
        if (SteeringAction)
        {
            EnhancedInputComponent->BindAction(SteeringAction, ETriggerEvent::Triggered, this, &AMyWheeledVehiclePawn::ApplySteering);
            EnhancedInputComponent->BindAction(SteeringAction, ETriggerEvent::Completed, this, &AMyWheeledVehiclePawn::ApplySteering);
        }

        // El Freni
        if (HandbrakeAction)
        {
            EnhancedInputComponent->BindAction(HandbrakeAction, ETriggerEvent::Triggered, this, &AMyWheeledVehiclePawn::ApplyHandbrake);
            EnhancedInputComponent->BindAction(HandbrakeAction, ETriggerEvent::Completed, this, &AMyWheeledVehiclePawn::ApplyHandbrake);
        }

        // Sürücünün etrafa bakmasý
        if (LookAction)
        {
            EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyWheeledVehiclePawn::Look);
            EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Completed, this, &AMyWheeledVehiclePawn::Look);
        }

        // Ateþ Etme Tuþu
        if (FireAction)
        {
            EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &AMyWheeledVehiclePawn::Fire);
        }

        // Kamera Geçiþ Tuþu
        if (ChangeCameraAction)
        {
            EnhancedInputComponent->BindAction(ChangeCameraAction, ETriggerEvent::Started, this, &AMyWheeledVehiclePawn::ChangeCamera);
        }
    }
}

// SÜRÜCÜ FONKSÝYONLARI

void AMyWheeledVehiclePawn::ApplyThrottle(const FInputActionValue& Value)
{
    GetVehicleMovementComponent()->SetThrottleInput(Value.Get<float>());
}

void AMyWheeledVehiclePawn::ApplyBrake(const FInputActionValue& Value)
{
    GetVehicleMovementComponent()->SetBrakeInput(Value.Get<float>());
}

void AMyWheeledVehiclePawn::ApplySteering(const FInputActionValue& Value)
{
    float InputValue = Value.Get<float>();
    GetVehicleMovementComponent()->SetSteeringInput(InputValue);
    CurrentSteeringAngle = InputValue * 45.0f; // Tekerlek animasyonu için
}

void AMyWheeledVehiclePawn::ApplyHandbrake(const FInputActionValue& Value)
{
    GetVehicleMovementComponent()->SetHandbrakeInput(Value.Get<bool>());
}

void AMyWheeledVehiclePawn::Look(const FInputActionValue& Value)
{
    FVector2D LookAxisVector = Value.Get<FVector2D>();
    AddControllerYawInput(LookAxisVector.X);
    AddControllerPitchInput(LookAxisVector.Y);
}

// SERVER RPC (MULTIPLAYER SENKRONÝZASYON)

void AMyWheeledVehiclePawn::Fire(const FInputActionValue& Value)
{
    Server_Fire();
}

void AMyWheeledVehiclePawn::Server_SetTurretRotation_Implementation(float NewYaw, float NewPitch)
{
    TurretYaw = NewYaw;
    BarrelPitch = NewPitch;
}

bool AMyWheeledVehiclePawn::Server_SetTurretRotation_Validate(float NewYaw, float NewPitch)
{
    return true;
}

void AMyWheeledVehiclePawn::Server_Fire_Implementation()
{
    if (ProjectileClass)
    {
        FVector SpawnLocation;
        FRotator SpawnRotation;

        if (TopSkeletalMesh && TopSkeletalMesh->DoesSocketExist(MuzzleSocketName))
        {
            SpawnLocation = TopSkeletalMesh->GetSocketLocation(MuzzleSocketName);
            SpawnRotation = TopSkeletalMesh->GetSocketRotation(MuzzleSocketName);
        }
        else if (GetMesh() && GetMesh()->DoesSocketExist(MuzzleSocketName))
        {
            SpawnLocation = GetMesh()->GetSocketLocation(MuzzleSocketName);
            SpawnRotation = GetMesh()->GetSocketRotation(MuzzleSocketName);
        }
        else
        {
            if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("HATA: MuzzleSocket bulunamadi!"));
            return;
        }

        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.Instigator = GetInstigator();

        GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);

        Multicast_PlayFireEffects();
    }
}

bool AMyWheeledVehiclePawn::Server_Fire_Validate()
{
    return true; 
}

// REP-NOTIFY VE ÝSTEMCÝ (CLIENT) FONKSÝYONLARI


void AMyWheeledVehiclePawn::OnRep_CurrentRole()
{
    UpdateCameraMode();

    if (GEngine)
    {
        FString RoleName = (CurrentRole == EArmorRole::Driver)
            ? TEXT("Surucu (Driver)")
            : TEXT("Nisanci (Gunner)");

        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green,
            FString::Printf(TEXT("Aktif Rol: %s"), *RoleName));
    }
}

void AMyWheeledVehiclePawn::PawnClientRestart()
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

void AMyWheeledVehiclePawn::UpdateCameraMode()
{
    USpringArmComponent* SpringArm = FindComponentByClass<USpringArmComponent>();
    if (!SpringArm) return;

    if (CurrentRole == EArmorRole::Gunner)
    {
        SpringArm->bUsePawnControlRotation = true;
    }
    else
    {
        SpringArm->bUsePawnControlRotation = false;
    }
}

void AMyWheeledVehiclePawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void AMyWheeledVehiclePawn::BeginPlay()
{
    Super::BeginPlay();

    TArray<UCameraComponent*> Cameras;
    GetComponents<UCameraComponent>(Cameras);

    for (UCameraComponent* Cam : Cameras)
    {
        if (Cam->GetName().Contains(TEXT("FPC")))
        {
            FPCamera = Cam;
            FPCamera->Deactivate(); 
        }
        // "TPC" (Third Person) içeren kamerayý bul
        else if (Cam->GetName().Contains(TEXT("TPC")))
        {
            TPCamera = Cam;
            TPCamera->Activate();
        }
    }

    TArray<USkeletalMeshComponent*> SkeletalMeshes;
    GetComponents<USkeletalMeshComponent>(SkeletalMeshes);

    for (USkeletalMeshComponent* SM : SkeletalMeshes)
    {
        if (SM != GetMesh())
        {
            TopSkeletalMesh = SM;
            break; 
        }
    }
}

void AMyWheeledVehiclePawn::ChangeCamera(const FInputActionValue& Value)
{
    if (TPCamera && FPCamera)
    {
        if (TPCamera->IsActive())
        {
            TPCamera->Deactivate();
            FPCamera->Activate();
        }
    
        else
        {
            FPCamera->Deactivate();
            TPCamera->Activate();
        }
    }
}

void AMyWheeledVehiclePawn::Multicast_PlayFireEffects_Implementation()
{

    if (FireSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
    }


    if (MuzzleFlashEffect && TopSkeletalMesh)
    {
        UGameplayStatics::SpawnEmitterAttached(
            MuzzleFlashEffect,
            TopSkeletalMesh,
            MuzzleSocketName,
            FVector::ZeroVector,
            FRotator::ZeroRotator,
            EAttachLocation::SnapToTarget,
            true
        );
    }
}