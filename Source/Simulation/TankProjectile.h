// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TankProjectile.generated.h"

UCLASS()
class SIMULATION_API ATankProjectile : public AActor
{
    GENERATED_BODY()

public:
    ATankProjectile();

protected:
    virtual void BeginPlay() override;

public:

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class USphereComponent* CollisionComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UStaticMeshComponent* ProjectileMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    class UProjectileMovementComponent* ProjectileMovement;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
    float BaseDamage;

    UFUNCTION()
    void OnHit(class UPrimitiveComponent* HitComp, AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const struct FHitResult& Hit);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
    class UParticleSystem* HitParticles;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
    class USoundBase* HitSound;
};