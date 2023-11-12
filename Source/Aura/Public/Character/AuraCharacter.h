// Copyright Axchemy Games

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "AuraCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()
	
public:
	AAuraCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	/** UPlayerInterface */
	virtual int32 GetXP_Implementation() const override;
	virtual void AddToXP_Implementation(int32 XPToAdd) override;
	virtual void LevelUp_Implementation() override;
	virtual int32 FindLevelForXP_Implementation(int32 InXP) const override;
	/** End UPlayerInterface */

	/* Combat Interface*/
	virtual int32 GetPlayerLevel_Implementation() override;
	/* End Combat Interface*/
	
private:
	virtual void InitAbilityActorInfo() override;
};
