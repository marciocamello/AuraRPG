// Copyright Axchemy Games

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interaction/FadeInterface.h"
#include "FadeObjectComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSetFadeOut);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSetFadeIn);

UCLASS(Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent))
class AURA_API UFadeObjectComponent : public UActorComponent, public IFadeInterface
{
	GENERATED_BODY()

public:
	UFadeObjectComponent();
	
	UFUNCTION(BlueprintCallable)
	void ResetMaterials();

	UFUNCTION(BlueprintCallable, Category="Default")
	void FadeFinish(double Intensity);

	UFUNCTION(BlueprintCallable, Category="Default")
	void SetMaterialToDynamicInstances();

	UFUNCTION(BlueprintCallable)
	void SetupMaterials();

	/*UFadeInterface*/
	virtual void FadeOut_Implementation() override;
	virtual void FadeIn_Implementation() override;
	/*end UFadeInterface*/

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	float AnimationProgress = 1.0f;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	TArray<UMaterialInterface*> OriginalMaterials;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	TArray<UMaterialInstanceDynamic*> DynamicMaterialInstances;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Fade Properties")
	TArray<UMaterialInstance*> FadeMaterialInstances;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Fade Properties")
	bool bBlockVisibility;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	FTimerHandle FadeOutTimer;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	FTimerHandle FadeInTimer;

	UFUNCTION(BlueprintCallable)
	void SetFadeOut();

	UFUNCTION(BlueprintCallable)
	void SetFadeIn();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Fade Properties")
	float FadeTime = 0.004f;

protected:
	virtual void BeginPlay() override;

};
