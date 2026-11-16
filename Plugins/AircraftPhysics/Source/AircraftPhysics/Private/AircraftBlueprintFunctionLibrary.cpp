#include "AircraftBlueprintFunctionLibrary.h"

float UAircraftBlueprintFunctionLibrary::EstimateMaxSpeed(float InMass, float InAeroPower, float InAeroDrag)
{
	const float MaxSpeed = FMath::Sqrt(InMass * InAeroPower / FMath::Max(InAeroDrag, 0.01f));
	return MaxSpeed;
}

float UAircraftBlueprintFunctionLibrary::EstimateRateOfClimb(float InAeroPower)
{
	const float RateOfClimb = InAeroPower * 0.3f * 75.f / 12.25f;
	return RateOfClimb;
}

float UAircraftBlueprintFunctionLibrary::EstimateMaxAltitude(float InAeroAltitude)
{
	const float MaxAltitude = FMath::Max(InAeroAltitude, 0.1f);
	return MaxAltitude;
}

float UAircraftBlueprintFunctionLibrary::EstimateTurnTime(float InMass, FVector InInertiaTensor, float InAngularDrag, float InAeroPitch, float InAeroResistance, float InAeroMult)
{
	const float K = InMass * InAeroMult / FMath::Max(InInertiaTensor.Y, 0.01f);

	const float T = 0.5f * InAeroPitch * K;
	const float D = InAeroResistance * K;

	const float MaxTurnSpeed = T / FMath::Max(0.01f, InAngularDrag + D);

	const float TurnTime = 2.f * PI / FMath::Max(0.01f, MaxTurnSpeed);

	return TurnTime;
}

float UAircraftBlueprintFunctionLibrary::EstimateRollTime(float InMass, FVector InInertiaTensor, float InAngularDrag, float InAeroRoll, float InAeroResistance, float InAeroMult)
{
	const float K = InMass * InAeroMult / FMath::Max(InInertiaTensor.X, 0.01f);

	const float T = 0.5f * InAeroRoll * K;
	const float D = InAeroResistance * K;

	const float MaxTurnSpeed = T / FMath::Max(0.01f, InAngularDrag + D);

	const float RollTime = 2.f * PI / FMath::Max(0.01f, MaxTurnSpeed);

	return RollTime;
}

float UAircraftBlueprintFunctionLibrary::EstimateYawTime(float InMass, FVector InInertiaTensor, float InAngularDrag, float InAeroYaw, float InAeroResistance, float InAeroMult)
{
	const float K = InMass * InAeroMult / FMath::Max(InInertiaTensor.Z, 0.01f);

	const float T = 0.5f * InAeroYaw * K;
	const float D = InAeroResistance * K;

	const float MaxTurnSpeed = T / FMath::Max(0.01f, InAngularDrag + D);

	const float YawTime = 2.f * PI / FMath::Max(0.01f, MaxTurnSpeed);

	return YawTime;
}

float UAircraftBlueprintFunctionLibrary::EstimateTurnRadius(float InMass, float InAeroPower, float InAeroDrag, FVector InInertiaTensor, float InAngularDrag, float InAeroPitch, float InAeroResistance, float InAeroMult)
{
	const float K = InMass * InAeroMult / FMath::Max(InInertiaTensor.Y, 0.01f);

	const float T = 0.5f * InAeroPitch * K;
	const float D = InAeroResistance * K;

	const float MaxTurnSpeed = T / FMath::Max(0.01f, InAngularDrag + D);
	const float MaxSpeed = FMath::Sqrt(InMass * InAeroPower / FMath::Max(InAeroDrag, 0.01f));

	const float TurnRadius = MaxSpeed / FMath::Max(MaxTurnSpeed, 0.01f);

	return TurnRadius;
}