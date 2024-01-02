// Copyright Axchemy Games


#include "Actor/AuraProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Aura/Aura.h"
#include "Components/AudioComponent.h"

AAuraProjectile::AAuraProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(Sphere);
	Sphere->SetCollisionObjectType(ECC_Projectile);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 550.f;
	ProjectileMovement->MaxSpeed = 550.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(LifeSpan);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraProjectile::OnSphereOverlap);
	LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent());
}

void AAuraProjectile::ApplyImpactEffects()
{
	UGameplayStatics::PlaySoundAtLocation(this,ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactEffect, GetActorLocation(), FRotator::ZeroRotator);
	if(LoopingSoundComponent != nullptr)
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}
	bHit = true;
}

void AAuraProjectile::Destroyed()
{
	if(LoopingSoundComponent != nullptr)
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}
	if(!bHit && !HasAuthority()) ApplyImpactEffects();
	Super::Destroyed();
}

void AAuraProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	if(SourceAvatarActor == OtherActor) return;
	if(!UAuraAbilitySystemLibrary::IsNotFriend(SourceAvatarActor, OtherActor)) return;
	if(!bHit) ApplyImpactEffects();
	
	if(HasAuthority())
	{
		if(UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			if(DamageType.IsValid())
			{
				const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DamageType[DamageType].DeathImpulseMagnitude.Value;
				DamageEffectParams.DamageType[DamageType].DeathImpulse = DeathImpulse;

				const bool bKnockBack = FMath::RandRange(1, 100) < DamageEffectParams.DamageType[DamageType].KnockBackChance.Value;
				if(bKnockBack)
				{
					FRotator Rotation = GetActorRotation();
					Rotation.Pitch = 45.f;
					
					const FVector KnockBackDirection = Rotation.Vector();
					const FVector KnockBackForce = KnockBackDirection * DamageEffectParams.DamageType[DamageType].KnockBackForceMagnitude.Value;
					DamageEffectParams.DamageType[DamageType].KnockBackForce = KnockBackForce;
				}
			}
			
			DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
			UAuraAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
		}
		
		Destroy();
	}
	else bHit = true;
}
