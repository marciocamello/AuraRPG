// Copyright Axchemy Games


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/AuraPlayerState.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();
	SpellPointsChanged.Broadcast(GetAuraPlayerState()->GetSpellPoints());
}

void USpellMenuWidgetController::BindCallbackToDependencies()
{
	GetAuraAbilitySystemComponent()->AbilityStatusChanged.AddLambda(
		[this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 NewLevel)
		{

			if(SelectedAbility.Ability.MatchesTagExact(AbilityTag))
			{
				SelectedAbility.Status = StatusTag;
				bool bEnableSpendPoints = false;
				bool bEnableEquip = false;
				ShouldEnableButtons(StatusTag, CurrentSpellPoints, bEnableSpendPoints, bEnableEquip);
				FString Description;
				FString NextLevelDescription;
				GetAuraAbilitySystemComponent()->GetDescriptionsByAbilityTag(AbilityTag, Description, NextLevelDescription);
				SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, Description, NextLevelDescription);
			}
			
			if(AbilityInfo)
			{
				FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
				Info.StatusTag = StatusTag;
				AbilityInfoDelegate.Broadcast(Info);
			}
		}
	);

	GetAuraAbilitySystemComponent()->AbilityEquipped.AddUObject(this, &USpellMenuWidgetController::OnAbilityEquipped);

	GetAuraPlayerState()->OnSpellPointsChangedDelegate.AddLambda(
		[this](int32 SpellPoints)
		{
			SpellPointsChanged.Broadcast(SpellPoints);
			CurrentSpellPoints = SpellPoints;
			
			bool bEnableSpendPoints = false;
			bool bEnableEquip = false;
			ShouldEnableButtons(SelectedAbility.Status, CurrentSpellPoints, bEnableSpendPoints, bEnableEquip);
			FString Description;
			FString NextLevelDescription;
			GetAuraAbilitySystemComponent()->GetDescriptionsByAbilityTag(SelectedAbility.Ability, Description, NextLevelDescription);
			SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, Description, NextLevelDescription);
		}
	);
}

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	if(bWaitingForEquipSelection)
	{
		const FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(AbilityTag).AbilityType;
		StopWaitingForEquipDelegate.Broadcast(SelectedAbilityType);
		bWaitingForEquipSelection = false;
	}
	
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	const int32 SpellPoints = GetAuraPlayerState()->GetSpellPoints();
	FGameplayTag AbilityStatus;

	const bool bTagValid = AbilityTag.IsValid();
	const bool bTagNone = AbilityTag.MatchesTag(GameplayTags.Abilities_None);
	const FGameplayAbilitySpec* AbilitySpec = GetAuraAbilitySystemComponent()->GetSpecFromAbilityTag(AbilityTag);
	const bool bSpecValid = AbilitySpec != nullptr;
	
	if(!bTagValid || bTagNone || !bSpecValid)
	{
		AbilityStatus = GameplayTags.Abilities_Status_Locked;
	}
	else
	{
		AbilityStatus = GetAuraAbilitySystemComponent()->GetStatusFromSpec(*AbilitySpec); 
	}
	
	SelectedAbility.Ability = AbilityTag;
	SelectedAbility.Status = AbilityStatus;
	bool bEnableSpendPoints = false;
	bool bEnableEquip = false;
	ShouldEnableButtons(AbilityStatus, SpellPoints, bEnableSpendPoints, bEnableEquip);
	FString Description;
	FString NextLevelDescription;
	GetAuraAbilitySystemComponent()->GetDescriptionsByAbilityTag(AbilityTag, Description, NextLevelDescription);
	SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, Description, NextLevelDescription);
}

void USpellMenuWidgetController::SpendPointButtonPressed()
{
	if(GetAuraAbilitySystemComponent())
	{
		GetAuraAbilitySystemComponent()->ServerSpendSpellPoint(SelectedAbility.Ability);
	}
}

void USpellMenuWidgetController::GlobeDeselect()
{
	if(bWaitingForEquipSelection)
	{
		const FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;
		StopWaitingForEquipDelegate.Broadcast(SelectedAbilityType);
		bWaitingForEquipSelection = false;
	}
	
	SelectedAbility.Ability = FAuraGameplayTags::Get().Abilities_None;
	SelectedAbility.Status = FAuraGameplayTags::Get().Abilities_Status_Locked;
	
	SpellGlobeSelectedDelegate.Broadcast(false, false, FString(), FString());
}

void USpellMenuWidgetController::EquipButtonPressed()
{
	const FGameplayTag AbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;
	WaitForEquipSelectionDelegate.Broadcast(AbilityType);
	bWaitingForEquipSelection = true;

	const FGameplayTag SelectedStatus = GetAuraAbilitySystemComponent()->GetStatusFromAbilityTag(SelectedAbility.Ability);
	if(SelectedStatus.MatchesTagExact(FAuraGameplayTags::Get().Abilities_Status_Equipped))
	{
		SelectedSlot = GetAuraAbilitySystemComponent()->GetInputTagFromAbilityTag(SelectedAbility.Ability);	
	}
}

void USpellMenuWidgetController::SpellRowGlobePressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType)
{
	if(!bWaitingForEquipSelection) return;
	// check selected ability against the slot's ability type.
	// (don't equip an offensive ability in a passive slot and vice versa)
	const FGameplayTag& SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;
	if(!SelectedAbilityType.MatchesTagExact(AbilityType)) return;

	GetAuraAbilitySystemComponent()->ServerEquipAbility(SelectedAbility.Ability, SlotTag); 
}

void USpellMenuWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status,
	const FGameplayTag& Slot, FGameplayTag PreviousSlot)
{
	bWaitingForEquipSelection = false;

	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	
	FAuraAbilityInfo LastSlotInfo;
	LastSlotInfo.StatusTag = GameplayTags.Abilities_Status_Unlocked;
	LastSlotInfo.InputTag = PreviousSlot;
	LastSlotInfo.AbilityTag = GameplayTags.Abilities_None;
	// broadcast empty info if PreviousSlot is a valid. Only if equipping on already-equipped spell
	AbilityInfoDelegate.Broadcast(LastSlotInfo);

	FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	Info.StatusTag = Status;
	Info.InputTag = Slot;
	AbilityInfoDelegate.Broadcast(Info);

	StopWaitingForEquipDelegate.Broadcast(AbilityInfo->FindAbilityInfoForTag(AbilityTag).AbilityType);
	SpellGlobeReassignedDelegate.Broadcast(AbilityTag);
	GlobeDeselect();
}

void USpellMenuWidgetController::ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 SpellPoints,
                                                     bool& bShouldEnableSpellPointsButton, bool& bShouldEnableEquipButton)
{
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

	bShouldEnableSpellPointsButton = false;
	bShouldEnableEquipButton = false;
	if(AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Equipped))
	{
		bShouldEnableEquipButton = true;
		if(SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton = true;
		}
	}
	else if(AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Eligible))
	{
		if(SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton = true;
		}
	}
	else if(AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))
	{
		bShouldEnableEquipButton = true;
		if(SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton = true;
		}
	}
}
