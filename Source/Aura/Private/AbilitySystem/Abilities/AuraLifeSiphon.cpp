// Copyright Axchemy Games

#include "AbilitySystem/Abilities/AuraLifeSiphon.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"

UAuraLifeSiphon::UAuraLifeSiphon()
{
	PassiveType = EPassiveAbilityType::LifeSteal;
}

FString UAuraLifeSiphon::GetSpecificEffectLine(int32 Level)
{
	const int32 LifeSteal = PassiveAbilityCurve.GetValueAtLevel(Level);
	return FString::Printf(TEXT("<Small>Life Steal:</> <Highlight>%d%%</>"), LifeSteal);
}

void UAuraLifeSiphon::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
									  const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
									  const FGameplayEventData *TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (UAuraAbilitySystemComponent *AuraASC = Cast<UAuraAbilitySystemComponent>(
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo())))
	{
		AuraASC->AddLooseGameplayTag(FAuraGameplayTags::Get().Abilities_Passive_LifeSiphon);
	}
}

void UAuraLifeSiphon::EndAbility(const FGameplayAbilitySpecHandle Handle,
								 const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
								 bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (UAuraAbilitySystemComponent *AuraASC = Cast<UAuraAbilitySystemComponent>(
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo())))
	{
		AuraASC->RemoveLooseGameplayTag(FAuraGameplayTags::Get().Abilities_Passive_LifeSiphon);
	}
}
