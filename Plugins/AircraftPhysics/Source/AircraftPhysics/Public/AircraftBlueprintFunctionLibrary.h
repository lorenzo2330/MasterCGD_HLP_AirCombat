#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AircraftBlueprintFunctionLibrary.generated.h"

UCLASS()
class AIRCRAFTPHYSICS_API UAircraftBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "Aircraft")
	static float MetersPerSecondToKnots(float InValue);
	UFUNCTION(BlueprintCallable, Category = "Aircraft")
	static float MetersToFeet(float InValue);

	UFUNCTION(BlueprintCallable, Category = "Aircraft")
	static float EstimateMaxSpeed(float InMass, float InAeroPower, float InAeroDrag);
	UFUNCTION(BlueprintCallable, Category = "Aircraft")
	static float EstimateRateOfClimb(float InAeroPower);
	UFUNCTION(BlueprintCallable, Category = "Aircraft")
	static float EstimateMaxAltitude(float InAeroAltitude);
	UFUNCTION(BlueprintCallable, Category = "Aircraft")
	static float EstimateTurnTime(float InMass, FVector InInertiaTensor, float InAngularDrag, float InAeroPitch, float InAeroResistance, float InAeroMult);
	UFUNCTION(BlueprintCallable, Category = "Aircraft")
	static float EstimateRollTime(float InMass, FVector InInertiaTensor, float InAngularDrag, float InAeroRoll, float InAeroResistance, float InAeroMult);
	UFUNCTION(BlueprintCallable, Category = "Aircraft")
	static float EstimateYawTime(float InMass, FVector InInertiaTensor, float InAngularDrag, float InAeroYaw, float InAeroResistance, float InAeroMult);
	UFUNCTION(BlueprintCallable, Category = "Aircraft")
	static float EstimateTurnRadius(float InMass, float InAeroPower, float InAeroDrag, FVector InInertiaTensor, float InAngularDrag, float InAeroPitch, float InAeroResistance, float InAeroMult);
};

FORCEINLINE_DEBUGGABLE float UAircraftBlueprintFunctionLibrary::MetersPerSecondToKnots(float InValue)
{
	return InValue * 1.94f;
}

FORCEINLINE_DEBUGGABLE float UAircraftBlueprintFunctionLibrary::MetersToFeet(float InValue)
{
	return InValue * 3.28f;
}