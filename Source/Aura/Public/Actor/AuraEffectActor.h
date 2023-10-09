// Copyright Axchemy Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "AuraEffectActor.generated.h"

class UAbilitySystemComponent;
class UGameplayEffect;

UENUM(BlueprintType)
enum class EEffectApplicationPolicy
{
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DoNotApply
};

UENUM(BlueprintType)
enum class EEffectRemovalPolicy
{
	RemoveEndOverlap,
	DoNotRemove
};

USTRUCT(BlueprintType)
struct FGameplayEffectProtocol
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (InlineEditConditionToggle))
	bool bHasEffectApplicationPolicy;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bHasEffectApplicationPolicy"))
	EEffectApplicationPolicy GameplayEffectApplicationPolicy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (InlineEditConditionToggle))
	bool bHasEffectRemovalPolicy;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bHasEffectRemovalPolicy"))
	EEffectRemovalPolicy GameplayEffectRemovalPolicy;
};

UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraEffectActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Applied Effects")
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UFUNCTION(BlueprintCallable, Category = "Applied Effects")
	void OnOverlap(AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category = "Applied Effects")
	void OnEndOverlap(AActor* TargetActor);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	bool bDestroyOnEffectRemoval = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TMap<TSubclassOf<UGameplayEffect>, FGameplayEffectProtocol> GameplayEffectList;

	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHandles;
};
