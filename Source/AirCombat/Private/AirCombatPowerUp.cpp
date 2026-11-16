#include "AirCombatPowerUp.h"

#include "ReflectionFunctionLibrary.h"

AAirCombatPowerUp::AAirCombatPowerUp()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAirCombatPowerUp::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &AAirCombatPowerUp::OnBeginOverlap);
}

void AAirCombatPowerUp::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	OnActorBeginOverlap.RemoveAll(this);
}

void AAirCombatPowerUp::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	// Completed: Search TargetProperty on OtherActor and apply Multiplier to it.
	
	if (!OtherActor)
	{
		return;
	}
	
	void* Target = nullptr;
	FProperty* Property = UReflectionFunctionLibrary::RetriveProperty(OtherActor, TargetProperty, Target);
	
	if (Property && Target)
	{
		float* Value = Property->ContainerPtrToValuePtr<float>(Target);
		*Value *= Multiplier;
	}

	Destroy();
}

