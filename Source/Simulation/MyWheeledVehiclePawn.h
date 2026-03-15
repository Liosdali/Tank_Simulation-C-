// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "InputActionValue.h"
#include "Components/PoseableMeshComponent.h"
#include "MyWheeledVehiclePawn.generated.h"

UENUM(BlueprintType)
enum class EArmorRole : uint8
{
    Driver,
    Gunner
};

UCLASS()
class SIMULATION_API AMyWheeledVehiclePawn : public AWheeledVehiclePawn
{
    GENERATED_BODY()

public:
    AMyWheeledVehiclePawn();

    // Að üzerinden deðiþken senkronizasyonu için gerekli 
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual void PawnClientRestart() override;
    virtual void BeginPlay() override;

    // Kontrol metodlarý
    void ApplyThrottle(const FInputActionValue& Value);
    void ApplyBrake(const FInputActionValue& Value);
    void ApplySteering(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
    void ApplyHandbrake(const FInputActionValue& Value);
    void Fire(const FInputActionValue& Value);

public:
    // MULTIPLAYER VE ROL DEÐÝÞKENLERÝ

    // Ýstemciler (client) ve sunucu arasýnda rol senkronizasyonu
    UPROPERTY(ReplicatedUsing = OnRep_CurrentRole, BlueprintReadOnly, Category = "Roles")
    EArmorRole CurrentRole;

    // Rol deðiþtiðinde ekrana bildirim basacak fonksiyon
    UFUNCTION()
    void OnRep_CurrentRole();

    // Multiplayer'da kulenin döndüðünü herkesin görmesi için Replicated olmalý 
    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Turret")
    float TurretYaw;

	//Ayný þekilde namlunun yukarý aþaðý hareketi de herkes tarafýndan görünmeli
    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Turret")
    float BarrelPitch;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Steering")
    float CurrentSteeringAngle;
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TSubclassOf<class AActor> ProjectileClass;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName MuzzleSocketName;

    // ENHANCED INPUT DEÐÝÞKENLERÝ

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    class UInputAction* ThrottleAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    class UInputAction* BrakeAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    class UInputAction* SteeringAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    class UInputAction* LookAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    class UInputAction* HandbrakeAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    class UInputAction* FireAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    class UInputMappingContext* DefaultMappingContext;

    // SERVER RPC (UZAKTAN PROSEDÜR ÇAÐRILARI)

    // Niþancý fareyi hareket ettirdiðinde kule açýsýný sunucuya ilet
    UFUNCTION(Server, Reliable, WithValidation)
    void Server_SetTurretRotation(float NewYaw, float NewPitch);

    //Mermiyi sunucu tarafýnda (Server Authority) oluþtur
    UFUNCTION(Server, Reliable, WithValidation)
    void Server_Fire();

    // KULE VE NAMLU KEMÝK DÖNDÜRME

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
    UPoseableMeshComponent* TurretMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
    FName TurretBoneName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
    UPoseableMeshComponent* BarrelMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
    FName BarrelBoneName;

    // KAMERA VE ROL GÜNCELLEME

    virtual void Tick(float DeltaTime) override;
    void UpdateCameraMode();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    class UInputAction* ChangeCameraAction;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
    class UCameraComponent* TPCamera;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
    class UCameraComponent* FPCamera;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
    class USkeletalMeshComponent* TopSkeletalMesh;

    void ChangeCamera(const FInputActionValue& Value);

    // --- GÖRSEL VE ÝÞÝTSEL EFEKTLER ---

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    class UParticleSystem* MuzzleFlashEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    class USoundBase* FireSound;

    UFUNCTION(NetMulticast, Unreliable)
    void Multicast_PlayFireEffects();
};