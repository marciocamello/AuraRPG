// Copyright Axchemy Games


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Player/AuraPlayerState.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	check(AttributeInfo);
	for(FAuraAttributeInfo& Tag : AttributeInfo.Get()->AttributeInformation)
	{
		BroadcastAttributeInfo(Tag.AttributeTag);
	}
	
	AttributePointsChangedDelegate.Broadcast(GetAuraPlayerState()->GetAttributePoints());
}

void UAttributeMenuWidgetController::BindCallbackToDependencies()
{
	check(AttributeInfo);
	for(FAuraAttributeInfo& Tag : AttributeInfo.Get()->AttributeInformation)
	{
		GetAuraAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(Tag.AttributeGetter).AddLambda(
			[this, Tag](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Tag.AttributeTag);
			}
		);
	}
	
	GetAuraPlayerState()->OnAttributePointsChangedDelegate.AddLambda(
		[this](int32 AttributePoints)
		{
			AttributePointsChangedDelegate.Broadcast(AttributePoints);
		}
	);
	
}

void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	GetAuraAbilitySystemComponent()->UpgradeAttribute(AttributeTag);
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& Tag) const
{
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Tag);
	Info.AttributeValue = Info.AttributeGetter.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
