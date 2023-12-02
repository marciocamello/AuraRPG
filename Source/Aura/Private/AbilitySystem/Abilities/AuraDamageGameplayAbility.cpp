// Copyright Axchemy Games


#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UAuraDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	const FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);
	for(TTuple<FGameplayTag, FAuraDamageGameplayEffect> Pair : DamageType)
	{
		const float ScaledDamage = Pair.Value.Damage.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, Pair.Key, ScaledDamage);
	}
	
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}

FDamageEffectParams UAuraDamageGameplayAbility::MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor) const
{
	FDamageEffectParams Params;
	Params.WorldContextObject = GetAvatarActorFromActorInfo();
	Params.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	Params.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	Params.BaseDamage = GetDamageByDamageType(GetAbilityLevel(), FGameplayTag::EmptyTag);
	Params.AbilityLevel = GetAbilityLevel();
	Params.DamageType = DamageType;
	if(IsValid(TargetActor))
	{
		FRotator Rotation = (TargetActor->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).Rotation();
		Rotation.Pitch = 45.f;
		const FVector ToTarget = Rotation.Vector();
		for(TTuple<FGameplayTag, FAuraDamageGameplayEffect> Pair : DamageType)
		{
			if (Params.DamageType.Contains(Pair.Key) && DamageType.Contains(Pair.Key))
			{
				Params.DamageType[Pair.Key].DeathImpulse = ToTarget * DamageType[Pair.Key].DeathImpulseMagnitude.Value;
				Params.DamageType[Pair.Key].KnockBackForce = ToTarget * DamageType[Pair.Key].KnockBackForceMagnitude.Value;
			}
		}
	}
	
	return Params;
}

FTaggedMontage UAuraDamageGameplayAbility::GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const
{
	if(TaggedMontages.Num() > 0)
	{
		const int32 Selection = FMath::RandRange(0, TaggedMontages.Num() - 1);
		return TaggedMontages[Selection];
	}

	return FTaggedMontage();
}

float UAuraDamageGameplayAbility::GetDamageByDamageType(float InLevel, const FGameplayTag& DamageTypeTag) const
{
	for(TTuple<FGameplayTag, FAuraDamageGameplayEffect> Pair : DamageType)
	{
		if(DamageTypeTag == Pair.Key)
		{
			return Pair.Value.Damage.GetValueAtLevel(InLevel);
		}
	}
	return 0.f;
}
