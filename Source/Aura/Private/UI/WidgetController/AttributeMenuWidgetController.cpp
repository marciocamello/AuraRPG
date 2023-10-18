// Copyright Axchemy Games


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	//UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);

	check(AttributeInfo);

	for(FAuraAttributeInfo& Tag : AttributeInfo.Get()->AttributeInformation)
	{
		BroadcastAttributeInfo(Tag.AttributeTag);
	}
}

void UAttributeMenuWidgetController::BindCallbackToDependencies()
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	
	check(AttributeInfo);
	for(FAuraAttributeInfo& Tag : AttributeInfo.Get()->AttributeInformation)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Tag.AttributeGetter).AddLambda(
			[this, Tag](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Tag.AttributeTag);
			}
		);
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& Tag) const
{
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Tag);
	Info.AttributeValue = Info.AttributeGetter.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
