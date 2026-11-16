#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "AircraftTelemetryComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AIRCRAFTPHYSICS_API UAircraftTelemetryComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	// CTOR

	UAircraftTelemetryComponent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// BUSINESS LOGIC

	void DrawTelemetry(UCanvas* Canvas, float& YL, float& YPos);

private:

	void UpdateTelemetryTargets();

	// INTERNALS

	FString						CurrentTelemetryList;

	TArray<FString>				TelemetryArray;

	struct FTelemetryTarget
	{
		void*					TargetObject;
		FProperty*				TargetProperty;
	};

	TArray<FTelemetryTarget>	TelemetryTargets;

	TArray<TArray<float> >		TelemetryGraphValues;
	int32						TelemetryGraphIndex;
};
