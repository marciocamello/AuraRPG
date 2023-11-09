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
	if(Intensity >= 1.0f)
	{
		ResetMaterials();

		if(bBlockVisibility)
		{
			Mesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
		}
	}
	else
	{
		if(Intensity <= 0.0f)
		{
			if(bBlockVisibility)
			{
				Mesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
			}
		}
	}
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
		DynamicMaterialInstances.AddUnique(DynamicMaterialInstance);
	}
}

void UFadeObjectComponent::FadeOut_Implementation()
{
	SetMaterialToDynamicInstances();
	GetWorld()->GetTimerManager().SetTimer(FadeOutTimer, this, &UFadeObjectComponent::SetFadeOut, FadeTime, true);
}

void UFadeObjectComponent::FadeIn_Implementation()
{
	GetWorld()->GetTimerManager().SetTimer(FadeOutTimer, this, &UFadeObjectComponent::SetFadeIn, FadeTime, true);
}

void UFadeObjectComponent::SetFadeOut()
{
	if(AnimationProgress >= 0.0f)
	{
		AnimationProgress -= 0.01f;
		if(AnimationProgress <= 0.0f)
		{
			GetWorld()->GetTimerManager().ClearTimer(FadeOutTimer);
		}
		else
		{
			for (int32 Index = 0; Index < DynamicMaterialInstances.Num(); Index++)
			{
				UMaterialInstanceDynamic* DynamicMaterialInstance = DynamicMaterialInstances[Index];
				DynamicMaterialInstance->SetScalarParameterValue(FName("Fade"), AnimationProgress);
			}
		}
	}
}

void UFadeObjectComponent::SetFadeIn()
{
	AnimationProgress += 0.01f;
	if(AnimationProgress >= 1.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(FadeOutTimer);
		FadeFinish(AnimationProgress);
	}
	else
	{
		for (int32 Index = 0; Index < DynamicMaterialInstances.Num(); Index++)
		{
			UMaterialInstanceDynamic* DynamicMaterialInstance = DynamicMaterialInstances[Index];
			DynamicMaterialInstance->SetScalarParameterValue(FName("Fade"), AnimationProgress);
		}
	}
}

void UFadeObjectComponent::BeginPlay()
{
	Super::BeginPlay();

	if(Mesh != nullptr)
	{
		SetupMaterials();
	}
	else
	{
		UStaticMeshComponent* StaticMeshComponent = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
		if(IsValid(StaticMeshComponent))
		{
			Mesh = StaticMeshComponent;
			SetupMaterials();
		}
	}
}
