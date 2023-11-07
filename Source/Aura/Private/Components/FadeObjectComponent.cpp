// Copyright Axchemy Games


#include "Components/FadeObjectComponent.h"

#include "Kismet/KismetMaterialLibrary.h"


UFadeObjectComponent::UFadeObjectComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UFadeObjectComponent::ResetMaterials()
{
	for (int32 Index = 0; Index < OriginalMaterials.Num(); Index++)
	{
		Mesh->CreateDynamicMaterialInstance(Index, OriginalMaterials[Index]);
	}
}

void UFadeObjectComponent::FadeFinish(double Intensity)
{
}

void UFadeObjectComponent::SetMaterialToDynamicInstances()
{
	for (int32 Index = 0; Index < DynamicMaterialInstances.Num(); Index++)
	{
		UMaterialInstanceDynamic* DynamicMaterialInstance = DynamicMaterialInstances[Index];
		Mesh->CreateDynamicMaterialInstance(Index, DynamicMaterialInstance);
	}
}

void UFadeObjectComponent::SetupMaterials()
{
	DynamicMaterialInstances.Empty();
	OriginalMaterials = Mesh->GetMaterials();
	for(UMaterialInstance*& FadeMaterialInstance : FadeMaterialInstances)
	{
		UMaterialInstanceDynamic* DynamicMaterialInstance = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), FadeMaterialInstance);
		DynamicMaterialInstances.Add(DynamicMaterialInstance);
	}
}

void UFadeObjectComponent::FadeOut_Implementation()
{
	IFadeInterface::FadeOut_Implementation();
}

void UFadeObjectComponent::FadeIn_Implementation()
{
	IFadeInterface::FadeIn_Implementation();
}

void UFadeObjectComponent::BeginPlay()
{
	Super::BeginPlay();

	
}
