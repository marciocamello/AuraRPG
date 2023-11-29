#include "AuraAbilityTypes.h"

bool FAuraGameplayEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	uint32 RepBits = 0;
	
	const bool bBaseSuccess = Super::NetSerialize(Ar, Map, bOutSuccess);

	if (Ar.IsSaving())
	{
		if (bIsBlockedHit)
		{
			RepBits |= 1 << 0;
		}
		if (bIsCriticalHit)
		{
			RepBits |= 1 << 1;
		}
		if(bIsSuccessfulDebuff)
		{
			RepBits |= 1 << 2;
		}
		if(DebuffDamage.Value > 0.f)
		{
			RepBits |= 1 << 3;
		}
		if(DebuffDuration.Value > 0.f)
		{
			RepBits |= 1 << 4;
		}
		if(DebuffFrequency.Value > 0.f)
		{
			RepBits |= 1 << 5;
		}
		if(DamageType.IsValid())
		{
			RepBits |= 1 << 6;
		}
	}
	
	if (Ar.IsLoading())
	{
		if (RepBits & (1 << 0))
		{
			Ar << bIsBlockedHit;
		}
		if (RepBits & (1 << 1))
		{
			Ar << bIsCriticalHit;
		}
		if (RepBits & (1 << 2))
		{
			Ar << bIsSuccessfulDebuff;
		}
		if (RepBits & (1 << 3))
		{
			Ar << DebuffDamage.Value;
		}
		if (RepBits & (1 << 4))
		{
			Ar << DebuffDuration.Value;
		}
		if (RepBits & (1 << 5))
		{
			Ar << DebuffFrequency.Value;
		}
		if (RepBits & (1 << 6))
		{
			if (Ar.IsLoading())
			{
				if(DamageType.IsValid())
				{
					DamageType = TSharedPtr<FGameplayTag>(new FGameplayTag());
				}
			}
			DamageType->NetSerialize(Ar, Map, bOutSuccess);
		}
	}

	return bBaseSuccess;
}
