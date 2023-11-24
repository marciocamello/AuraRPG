// Copyright Axchemy Games

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AbilityInfo.generated.h"

class UGameplayAbility;

USTRUCT(BlueprintType)
struct FAuraAbilityInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AbilityTag = FGameplayTag();
	
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag InputTag = FGameplayTag();
	
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag StatusTag = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag CooldownTag = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UTexture2D> Icon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UMaterialInterface> BackgroundMaterial = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 LevelRequirement = 1;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> Ability;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Ability Details")
	FText Title;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Ability Details", meta=(MultiLine=true))
	FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Ability Details", meta=(MultiLine=true))
	FText NextLevelDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Ability Details", meta=(MultiLine=true))
	FText LockedDescription;
};

/**
 * 
 */
UCLASS()
class AURA_API UAbilityInfo : public UDataAsset
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	FAuraAbilityInfo FindAbilityInfoForTag(const FGameplayTag& AbilityTag, bool bLogNotFound = false) const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Information")
	TArray<FAuraAbilityInfo> AbilityInformation;
};
