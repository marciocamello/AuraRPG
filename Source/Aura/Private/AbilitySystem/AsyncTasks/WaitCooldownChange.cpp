// Copyright Axchemy Games


#include "AbilitySystem/AsyncTasks/WaitCooldownChange.h"

#include "AbilitySystemComponent.h"

UWaitCooldownChange* UWaitCooldownChange::WaitCooldownChange(UAbilitySystemComponent* AbilitySystemComponent,
                                                             const FGameplayTag& InCooldownTag)
{
	UWaitCooldownChange* WaitCooldownChange = NewObject<UWaitCooldownChange>();
	WaitCooldownChange->ASC = AbilitySystemComponent;
	WaitCooldownChange->CooldownTag = InCooldownTag;

	if(!IsValid(AbilitySystemComponent) || !InCooldownTag.IsValid())
	{
		WaitCooldownChange->EndTask();
		return nullptr;
	}

	// To know when the cooldown has ended (CooldownTag has been removed)
	AbilitySystemComponent->RegisterGameplayTagEvent(InCooldownTag, EGameplayTagEventType::NewOrRemoved).AddUObject(
		WaitCooldownChange,
		&UWaitCooldownChange::CoolDownTagChanged
	);

	// To know when a cooldown effect has been applied
	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(
		WaitCooldownChange,
		&UWaitCooldownChange::OnActiveEffectAdded
	);

	return WaitCooldownChange;
}

void UWaitCooldownChange::EndTask()
{
	if(!IsValid(ASC)) return;
	ASC->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);

	SetReadyToDestroy();
	MarkAsGarbage();
}

void UWaitCooldownChange::CoolDownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount)
{
	if(NewCount == 0)
	{
		OnCooldownEnd.Broadcast(0.f);
	}
}

void UWaitCooldownChange::OnActiveEffectAdded(UAbilitySystemComponent* TargetASC,
	const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer AssetTags;
	SpecApplied.GetAllAssetTags(AssetTags);

	FGameplayTagContainer GrantedTags;
	SpecApplied.GetAllGrantedTags(GrantedTags);

	if(AssetTags.HasTagExact(CooldownTag) || GrantedTags.HasTagExact(CooldownTag))
	{
		const FGameplayEffectQuery GameplayEffectQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTag.GetSingleTagContainer());
		TArray<float> TimesRemaining = ASC->GetActiveEffectsTimeRemaining(GameplayEffectQuery);
		if(TimesRemaining.Num() > 0)
		{
			float TimeRemaining = TimesRemaining[0];
			for(int32 i = 1; i < TimesRemaining.Num(); i++)
			{
				if(TimesRemaining[i] > TimeRemaining)
				{
					TimeRemaining = TimesRemaining[i];
				}
			}
			
			OnCooldownStart.Broadcast(TimeRemaining);
		}
	}
}
