// Copyright Axchemy Games

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interaction/FadeInterface.h"
#include "FadeObjectComponent.generated.h"

UCLASS(Blueprintable, BlueprintType)
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
	void FadeOut_Implementation() override;
	void FadeIn_Implementation() override;
	/*end UFadeInterface*/

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	double AnimationProgress ;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	TArray<UMaterialInterface*> OriginalMaterials;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	TArray<UMaterialInstanceDynamic*> DynamicMaterialInstances;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Fade Properties")
	TArray<UMaterialInstance*> FadeMaterialInstances;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Fade Properties")
	bool BlockVisibility;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	FTimerHandle FadeOutTimer;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	FTimerHandle FadeInTimer;

protected:
	virtual void BeginPlay() override;

};
