// Copyright Axchemy Games


#include "Actor/AuraEffectActor.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneComponent"));
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	if (!TargetActor) { return; }
    
	if(UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
	{
		check(GameplayEffectClass);
		FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);
        
		const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, EffectContextHandle);        
		const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
		const bool bIsInfiniteEffect = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
        
		bool bHasEndOverlapPolicy = false;
		if(const FGameplayEffectProtocol* Protocol = GameplayEffectList.Find(GameplayEffectClass))
		{
			bHasEndOverlapPolicy = Protocol->bHasEffectRemovalPolicy && Protocol->GameplayEffectRemovalPolicy == EEffectRemovalPolicy::RemoveEndOverlap;
		}
		if (bIsInfiniteEffect && bHasEndOverlapPolicy)
		{
			ActiveEffectHandles.Add(ActiveEffectHandle, TargetASC);
		}
		
	}
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	if (!TargetActor) { return; }
	for (const TTuple<TSubclassOf<UGameplayEffect>, FGameplayEffectProtocol> Effect : GameplayEffectList)
	{
		if (Effect.Value.bHasEffectApplicationPolicy &&
			Effect.Value.GameplayEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
		{
			ApplyEffectToTarget(TargetActor, Effect.Key);
		}
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (!TargetActor) { return; }
	for (const TTuple<TSubclassOf<UGameplayEffect>, FGameplayEffectProtocol> Effect : GameplayEffectList)
	{
		if (Effect.Value.bHasEffectApplicationPolicy &&
			Effect.Value.GameplayEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
		{
			ApplyEffectToTarget(TargetActor, Effect.Key);
		}

		if (Effect.Value.bHasEffectRemovalPolicy &&
			Effect.Value.GameplayEffectRemovalPolicy == EEffectRemovalPolicy::RemoveEndOverlap)
		{
			UAbilitySystemComponent* TargetAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
			if(!IsValid(TargetAsc)) { return; }

			TArray<FActiveGameplayEffectHandle> EffectHandlesToRemove;
			for (const TTuple<FActiveGameplayEffectHandle, UAbilitySystemComponent*> HandlePair : ActiveEffectHandles)
			{
				if (TargetAsc == HandlePair.Value)
				{
					TargetAsc->RemoveActiveGameplayEffect(HandlePair.Key, 1);
					EffectHandlesToRemove.Add(HandlePair.Key);
				}
			}
			for (FActiveGameplayEffectHandle& Handle : EffectHandlesToRemove)
			{
				ActiveEffectHandles.FindAndRemoveChecked(Handle);
			}
		}
	}
}

