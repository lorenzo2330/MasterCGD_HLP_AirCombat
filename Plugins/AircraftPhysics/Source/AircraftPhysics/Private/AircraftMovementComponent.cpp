#include "AircraftMovementComponent.h"
#include "AircraftBlueprintFunctionLibrary.h"

#include "Components/InputComponent.h"

#include "DrawDebugHelpers.h"

const float s_Ground_Resistance = 0.f;
const float s_Ground_AngularResistance = 0.f;

float FromCmToMeters(float InValue)
{
	return InValue * 0.01f;
}

float FromMetersToCm(float InValue)
{
	return InValue * 100.f;
}

FVector FromCmToMeters(FVector InValue)
{
	return InValue * 0.01f;
}

FVector FromMetersToCm(FVector InValue)
{
	return InValue * 100.f;
}

float FromSquaredCmToSquaredMeters(float InValue)
{
	return InValue * 0.0001f;
}

float FromSquaredMetersToSquaredCm(float InValue)
{
	return InValue * 10000.f;
}

FVector FromSquaredCmToSquaredMeters(FVector InValue)
{
	return InValue * 0.0001f;
}

FVector FromSquaredMetersToSquaredCm(FVector InValue)
{
	return InValue * 10000.f;
}

UAircraftMovementComponent::UAircraftMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = ETickingGroup::TG_PrePhysics;

	DebugDrawTickFunction.bCanEverTick = true;
	DebugDrawTickFunction.TickGroup = ETickingGroup::TG_PostPhysics;
	DebugDrawTickFunction.bTickEvenWhenPaused = true;
	DebugDrawTickFunction.bStartWithTickEnabled = true;
}

void UAircraftMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();

	FBodyInstance* BodyInstance = UpdatedPrimitive->GetBodyInstance();

	if (BodyInstance == nullptr)
	{
		return;
	}

	BodyInstance->SetInstanceSimulatePhysics(true);
	BodyInstance->SetEnableGravity(false);

	BodyInstance->SetMassOverride(Mass, true);

	BodyInstance->InertiaTensorScale = InertiaTensor;

	BodyInstance->LinearDamping = Drag;
	BodyInstance->AngularDamping = AngularDrag;

	BodyInstance->UpdateMassProperties();
	BodyInstance->UpdateDampingProperties();
}

void UAircraftMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	RegisterDebugDrawTick(true);
}

void UAircraftMovementComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	RegisterDebugDrawTick(false);
}

void UAircraftMovementComponent::TickComponent(float InDeltaTime, enum ELevelTick InTickType, FActorComponentTickFunction* InThisTickFunction)
{
	Super::TickComponent(InDeltaTime, InTickType, InThisTickFunction);

	UpdateParameters(InDeltaTime);
	UpdateSimulation(InDeltaTime);
}

void UAircraftMovementComponent::ForceSpeed(float InPercentage)
{
	if (UpdatedPrimitive == nullptr)
	{
		return;
	}

	FBodyInstance* BodyInstance = UpdatedPrimitive->GetBodyInstance();

	if (BodyInstance == nullptr)
	{
		return;
	}

	const FTransform BodyTransform = BodyInstance->GetUnrealWorldTransform();
	const FQuat CurrentRotation = BodyTransform.GetRotation();

	const FVector CurrentForward = CurrentRotation.GetAxisX();

	const float EstimatedMaxSpeed = UAircraftBlueprintFunctionLibrary::EstimateMaxSpeed(Mass, AeroPower, AeroDrag);

	const float DesiredSpeed = EstimatedMaxSpeed * InPercentage;
	const FVector DesiredVelocity = DesiredSpeed * CurrentForward;

	BodyInstance->SetLinearVelocity(FromMetersToCm(DesiredVelocity), false);
}

void UAircraftMovementComponent::SetCrashed()
{
	Crashed = true;
}

void UAircraftMovementComponent::DebugDraw()
{
	if (UpdatedPrimitive == nullptr)
	{
		return;
	}

	FBodyInstance* BodyInstance = UpdatedPrimitive->GetBodyInstance();

	if (BodyInstance == nullptr)
	{
		return;
	}

	const FTransform BodyTransform = BodyInstance->GetUnrealWorldTransform();
	const FVector CurrentPosition = BodyTransform.GetTranslation();

	const FVector CurrentVelocity = BodyInstance->GetUnrealWorldVelocity();

	DrawDebugSphere(GetWorld(), CurrentPosition, 20.f, 32, FColor::White, false, -1.f, SDPG_Foreground);

	static float sVelocityScale = 1.f;
	DrawDebugLine(GetWorld(), CurrentPosition, CurrentPosition + CurrentVelocity * sVelocityScale, FColor::Yellow, false, -1.f, SDPG_Foreground);
}

void UAircraftMovementComponent::UpdateParameters(float InDeltaTime)
{
	MaxSpeed = UAircraftBlueprintFunctionLibrary::EstimateMaxSpeed(Mass, AeroPower, AeroDrag);
	MaxAltitude = UAircraftBlueprintFunctionLibrary::EstimateMaxAltitude(AeroAltitude);
	
	RateOfClimb = UAircraftBlueprintFunctionLibrary::EstimateRateOfClimb(AeroPower);

	TurnTime = UAircraftBlueprintFunctionLibrary::EstimateTurnTime(Mass, InertiaTensor, AngularDrag, AeroPitch, AeroResistance, AeroMult);
	RollTime = UAircraftBlueprintFunctionLibrary::EstimateRollTime(Mass, InertiaTensor, AngularDrag, AeroRoll, AeroResistance, AeroMult);
	YawTime = UAircraftBlueprintFunctionLibrary::EstimateYawTime(Mass, InertiaTensor, AngularDrag, AeroYaw, AeroResistance, AeroMult);

	TurnRadius = UAircraftBlueprintFunctionLibrary::EstimateTurnRadius(Mass, AeroPower, AeroDrag, InertiaTensor, AngularDrag, AeroPitch, AeroResistance, AeroMult);
}

void UAircraftMovementComponent::UpdateSimulation(float InDeltaTime)
{
	if (UpdatedPrimitive == nullptr)
	{
		return;
	}

	FBodyInstance* BodyInstance = UpdatedPrimitive->GetBodyInstance();

	if (BodyInstance == nullptr)
	{
		return;
	}

	const FVector Gravity = FVector(0.f, 0.f, FromCmToMeters(GetGravityZ()));

	const float BodyMass = BodyInstance->GetBodyMass();
	const FTransform BodyTransform = BodyInstance->GetUnrealWorldTransform();

	const FVector CurrentPosition = FromCmToMeters(BodyTransform.GetTranslation());
	const FQuat CurrentRotation = BodyTransform.GetRotation();

	const FVector CurrentEulerAngles = CurrentRotation.Euler();

	const FVector CurrentForward = CurrentRotation.GetAxisX();
	const FVector CurrentRight = CurrentRotation.GetAxisY();
	const FVector CurrentUp = CurrentRotation.GetAxisZ();

	const FVector CurrentVelocity = FromCmToMeters(BodyInstance->GetUnrealWorldVelocity());
	const FVector CurrentAngularVelocity = BodyInstance->GetUnrealWorldAngularVelocityInRadians();

	const FVector ForwardVelocity = CurrentVelocity.ProjectOnToNormal(CurrentForward);

	// Update state.

	Speed = CurrentVelocity.Size();
	ForwardSpeed = ForwardVelocity.Size();

	Altitude = CurrentPosition.Z;

	// Update simulation.

	FVector ForceToAdd = FVector::ZeroVector;
	FVector TorqueToAdd = FVector::ZeroVector;

	if (!Crashed)
	{
		// Update input.

		FVector CurrentControl = InputControl;
		CurrentControl.X = -InputControl.Z;

		const float CurrentThrottle = InputThrottle;

		ActualControl = ControlRate.InterpVector(InDeltaTime, ActualControl, CurrentControl);
		ActualThrottle = ThrottleRate.InterpValue(InDeltaTime, ActualThrottle, CurrentThrottle);

		// Update status.

		FVector RelativeVelocity = BodyTransform.InverseTransformVectorNoScale(CurrentVelocity);

		const float AeroFactor = FMath::Clamp(AeroF1 * (RelativeVelocity.X - AeroF2), 0.f, 1.f);
		const float AeroTorqueFactor = FMath::Clamp(2.f * AeroFactor, 0.f, 1.f);

		const float AltitudeFactor = AeroPowerPerf.GetRichCurveConst()->Eval(Altitude / FMath::Max(AeroAltitude, 0.1f), 1.f);

		// Compute force.

		FVector LocalAcceleration = -AeroForce * AeroFactor * RelativeVelocity;
		LocalAcceleration += AeroLift * AeroFactor * FVector::UpVector;								// Add lift
		LocalAcceleration.X = AeroPower * AltitudeFactor * ActualThrottle;							// Add thrust.

		FVector LocalForce = LocalAcceleration * BodyMass;
		LocalForce -= AeroDrag * RelativeVelocity * RelativeVelocity.Size();						// Add aerodynamic drag.

		FVector TotalForce = BodyTransform.TransformVectorNoScale(LocalForce);
		TotalForce += Gravity * BodyMass;

		ForceToAdd += TotalForce;

		// Compute torque.

		const float DotFU = FVector::DotProduct(CurrentForward, FVector::UpVector);
		const float DotRU = FVector::DotProduct(CurrentRight, FVector::UpVector);

		const FVector CrossFU = FVector::CrossProduct(CurrentForward, FVector::UpVector);

		const FVector CrossFF = -CrossFU * DotFU;
		const FVector CrossRR = -CurrentForward * DotRU;

		float InX = FMath::Clamp(ActualControl.X, -0.5f, 0.5f);
		float InY = FMath::Clamp(ActualControl.Y, -0.5f, 0.5f);
		float InZ = FMath::Clamp(ActualControl.Z, -0.5f, 0.5f);

		FVector AngularAcceleration = FVector::ZeroVector;

		AngularAcceleration += InY * AeroPitch * CurrentRight;		// Add pitch acceleration.
		AngularAcceleration += InX * AeroRoll * CurrentForward;		// Add roll acceleration.
		AngularAcceleration += InZ * AeroYaw * CurrentUp;			// Add yaw acceleration.

		AngularAcceleration += AeroLevelP * CrossFF;				// Re-aligning acceleration (pitch).
		AngularAcceleration += AeroLevelR * CrossRR;				// Re-aligning acceleration (roll).

		AngularAcceleration -= CurrentAngularVelocity * AeroResistance;
		AngularAcceleration *= AeroTorqueFactor;
		AngularAcceleration += AeroProj * FVector::CrossProduct(CurrentForward, CurrentVelocity);
		AngularAcceleration *= AeroMult;

		FVector TotalTorque = AngularAcceleration * BodyMass;

		TorqueToAdd += TotalTorque;
	}
	else
	{
		ForceToAdd += Gravity * BodyMass;
		ForceToAdd -= s_Ground_Resistance * BodyMass * CurrentVelocity;

		TorqueToAdd -= s_Ground_AngularResistance * BodyMass * CurrentAngularVelocity;
	}

	// Apply forces.

	BodyInstance->AddForce(FromMetersToCm(ForceToAdd));
	BodyInstance->AddTorqueInRadians(FromSquaredMetersToSquaredCm(TorqueToAdd));
}

void UAircraftMovementComponent::RegisterDebugDrawTick(bool bRegister)
{
	if (bRegister != DebugDrawTickFunction.IsTickFunctionRegistered())
	{
		if (bRegister)
		{
			if (SetupActorComponentTickFunction(&DebugDrawTickFunction))
			{
				DebugDrawTickFunction.Target = this;
			}
		}
		else
		{
			DebugDrawTickFunction.UnRegisterTickFunction();
		}
	}
}

void FAirctaftMovementComponentDebugDrawTickFunction::ExecuteTick(float DeltaTime, enum ELevelTick TickType, ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent)
{
	FActorComponentTickFunction::ExecuteTickHelper(Target, /*bTickInEditor=*/ false, DeltaTime, TickType, [this](float DilatedTime)
	{
		Target->DebugDraw();
	});
}

FString FAirctaftMovementComponentDebugDrawTickFunction::DiagnosticMessage()
{
	if (Target)
	{
		return Target->GetFullName() + TEXT("[DebugDraw]");
	}

	return TEXT("<NULL>[DebugDraw]");
}