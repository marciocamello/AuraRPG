// Copyright Axchemy Games

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AbilityInfo.generated.h"

class UGameplayAbility;

UENUM(BlueprintType)
enum EAuraAbilityInfoDescriptionType : uint8
{
	NormalInfo     UMETA(DisplayName = "Normal Info"),
	NextLevelInfo     UMETA(DisplayName = "Next Level Info"),
	LockedInfo    UMETA(DisplayName = "Locked Info"),
};

UENUM(BlueprintType)
enum EAuraAbilityInfoStylesType : uint8
{
	Default     UMETA(DisplayName = "Default"),
	Title	 UMETA(DisplayName = "Title"),
	Level     UMETA(DisplayName = "Level"),
	Damage     UMETA(DisplayName = "Damage"),
	Cooldown     UMETA(DisplayName = "Cooldown"),
	Percent	 UMETA(DisplayName = "Percent"),
	Name	 UMETA(DisplayName = "Name"),
	ManaCost    UMETA(DisplayName = "Mana Cost"),
	Small    UMETA(DisplayName = "Small"),
};

USTRUCT(BlueprintType)
struct FAuraAbilityInfoAbilityDetails
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TEnumAsByte<EAuraAbilityInfoStylesType> StyleType = EAuraAbilityInfoStylesType::Default;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(MultiLine=true))
	FText Text;
};

USTRUCT(BlueprintType)
struct FAuraAbilityInfoDescription
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TEnumAsByte<EAuraAbilityInfoDescriptionType> DescriptionType = EAuraAbilityInfoDescriptionType::NormalInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FAuraAbilityInfoAbilityDetails AbilityDetails;
};

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Ability Description")
	TArray<FAuraAbilityInfoDescription> Descriptions;
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
	
	UFUNCTION(BlueprintCallable)
	FAuraAbilityInfoDescription FindAbilityInfoDescriptionForType(const FGameplayTag& AbilityTag, const EAuraAbilityInfoDescriptionType& DescriptionType, bool bLogNotFound = false) const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Information")
	TArray<FAuraAbilityInfo> AbilityInformation;
};
