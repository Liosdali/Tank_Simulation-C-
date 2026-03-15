// Fill out your copyright notice in the Description page of Project Settings.

#include "TankProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h" 
#include "Sound/SoundBase.h"         

ATankProjectile::ATankProjectile()
{
    // Merminin að üzerindesenkronize olmasýný saðlar
    bReplicates = true;
    SetReplicateMovement(true);

    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    CollisionComp->InitSphereRadius(15.0f);
    CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
    CollisionComp->OnComponentHit.AddDynamic(this, &ATankProjectile::OnHit); 

    RootComponent = CollisionComp;

    ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
    ProjectileMesh->SetupAttachment(RootComponent);
    ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // Balistik bileþenler
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
    ProjectileMovement->UpdatedComponent = CollisionComp;
    ProjectileMovement->InitialSpeed = 8000.f; 
    ProjectileMovement->MaxSpeed = 8000.f;
    ProjectileMovement->bRotationFollowsVelocity = true; 
    ProjectileMovement->bShouldBounce = false;
    ProjectileMovement->ProjectileGravityScale = 1.0f; 

    BaseDamage = 50.0f;

	// Performans için merminin sahnede kalma süresini sýnýrlýdýr.
    InitialLifeSpan = 5.0f;
}

void ATankProjectile::BeginPlay()
{
    Super::BeginPlay();

    AActor* MyOwner = GetOwner();
    if (MyOwner && CollisionComp)
    {
        CollisionComp->IgnoreActorWhenMoving(MyOwner, true);
    }
}

void ATankProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    AActor* MyOwner = GetOwner();

    if ((OtherActor != nullptr) && (OtherActor != this) && (OtherActor != MyOwner) && (OtherComp != nullptr))
    {

        // GÖRSEL VE ÝÞÝTSEL EFEKTLER (Herkes Görebilsin Diye Sunucu Kontrolü Dýþýnda)
        if (HitParticles)
        {
            UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, GetActorTransform());
        }

        if (HitSound)
        {
            UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
        }

        // HASAR VE SÝLÝNME (Sadece Sunucu Otoritesi - Server Authority)
        if (HasAuthority())
        {
            UGameplayStatics::ApplyDamage(OtherActor, BaseDamage, GetInstigatorController(), this, UDamageType::StaticClass());

            if (GEngine)
            {
                GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("Vurulan hedef: %s |Verilen Hasar: %f"), *OtherActor->GetName(), BaseDamage));
            }
        }
    }

    // Çarptýktan sonra mermiyi sahneden silmeyi sadece sunucu yapr.
    if (HasAuthority())
    {
        Destroy();
    }
}