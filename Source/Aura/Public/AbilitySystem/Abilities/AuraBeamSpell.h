// Copyright Axchemy Games

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "AuraBeamSpell.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraBeamSpell : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, Category="Beam")
	void StoreMouseDataInfo(const FHitResult& HitResult);

	UFUNCTION(BlueprintCallable, Category="Beam")
	void StoreOwnerPlayerController();
protected:

	UPROPERTY(BlueprintReadWrite, Category="Beam")
	FVector MouseHitLocation;

	UPROPERTY(BlueprintReadWrite, Category="Beam")
	TObjectPtr<AActor> MouseHitActor;

	UPROPERTY(BlueprintReadWrite, Category="Beam")
	TObjectPtr<APlayerController> OwningPlayerController;
};
