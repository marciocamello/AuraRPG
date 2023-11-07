// Copyright Axchemy Games

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FadeInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UFadeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IFadeInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void FadeOut();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void FadeIn();
};
