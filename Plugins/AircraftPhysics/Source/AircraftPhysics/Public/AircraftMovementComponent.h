#pragma once

#include "CoreMinimal.h"
#include "GameFramework/MovementComponent.h"
#include "AircraftMovementComponent.generated.h"

class UInputComponent;

USTRUCT()
struct AIRCRAFTPHYSICS_API FAircraftInputRate
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = VehicleInputRate)
	float Rate;

	FAircraftInputRate() : Rate(1000.0f) { }

	float InterpValue(float InDeltaTime, float InCurrentValue, float InNewValue) const
	{
		const float DeltaValue = InNewValue - InCurrentValue;
		const float MaxDeltaValue = InDeltaTime * Rate;
		const float ClampedDeltaValue = FMath::Clamp(DeltaValue, -MaxDeltaValue, MaxDeltaValue);

		return InCurrentValue + ClampedDeltaValue;
	}

	FVector InterpVector(float InDeltaTime, FVector InCurrentValue, FVector InNewValue) const
	{
		return FVector(	InterpValue(InDeltaTime, InCurrentValue.X, InNewValue.X),
						InterpValue(InDeltaTime, InCurrentValue.Y, InNewValue.Y), 
						InterpValue(InDeltaTime, InCurrentValue.Z, InNewValue.Z));
	}
};

class UAircraftMovementComponent;

USTRUCT()
struct FAirctaftMovementComponentDebugDrawTickFunction : public FTickFunction
{
	GENERATED_BODY()

	UAircraftMovementComponent* Target = nullptr;

	virtual void ExecuteTick(float DeltaTime, enum ELevelTick TickType, ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent) override;
	virtual FString DiagnosticMessage() override;
};

template<>
struct TStructOpsTypeTraits<FAirctaftMovementComponentDebugDrawTickFunction> : public TStructOpsTypeTraitsBase2<FAirctaftMovementComponentDebugDrawTickFunction>
{
	enum
	{
		WithCopy = false
	};
};

UCLASS()
class AIRCRAFTPHYSICS_API UAircraftMovementComponent : public UMovementComponent
{
	GENERATED_BODY()

private:

	UPROPERTY(Category = "Body", EditAnywhere)
	float Mass = 1198.334f;
	UPROPERTY(Category = "Body", EditAnywhere)
	float Drag = 0.f;
	UPROPERTY(Category = "Body", EditAnywhere)
	float AngularDrag = 0.05f;
	UPROPERTY(Category = "Body", EditAnywhere)
	FVector InertiaTensor = FVector(4.905983f, 4.180213f, 2.525976f);

	UPROPERTY(Category = "Aerodynamics", EditAnywhere)
	float AeroF1 = 0.1f;
	UPROPERTY(Category = "Aerodynamics", EditAnywhere)
	float AeroF2 = 20.f;

	UPROPERTY(Category = "Power", EditAnywhere)
	float AeroForce = 3.75f;
	UPROPERTY(Category = "Power", EditAnywhere)
	float AeroLift = 9.81f;
	UPROPERTY(Category = "Power", EditAnywhere)
	float AeroPower = 5.771111f;
	UPROPERTY(Category = "Power", EditAnywhere)
	float AeroDrag = 0.7072f;

	UPROPERTY(Category = "Altitude", EditAnywhere)
	FRuntimeFloatCurve	AeroPowerPerf;
	UPROPERTY(Category = "Altitude", EditAnywhere)
	float AeroAltitude = 1720.f;

	UPROPERTY(Category = "Aerodynamics", EditAnywhere)
	float AeroPitch = 3.230929f;
	UPROPERTY(Category = "Aerodynamics", EditAnywhere)
	float AeroRoll = 4.840524f;
	UPROPERTY(Category = "Aerodynamics", EditAnywhere)
	float AeroYaw = 1.615464f;
	UPROPERTY(Category = "Aerodynamics", EditAnywhere)
	float AeroLevelP = 0.1615464f;
	UPROPERTY(Category = "Aerodynamics", EditAnywhere)
	float AeroLevelR = 2.420262f;
	UPROPERTY(Category = "Aerodynamics", EditAnywhere)
	float AeroProj = 0.006f;
	UPROPERTY(Category = "Aerodynamics", EditAnywhere)
	float AeroMult = 30.f;
	UPROPERTY(Category = "Aerodynamics", EditAnywhere)
	float AeroResistance = 3.f;

	UPROPERTY(Category = "Input", EditAnywhere)
	FAircraftInputRate ControlRate;
	UPROPERTY(Category = "Input", EditAnywhere)
	FAircraftInputRate ThrottleRate;

	UPROPERTY(Category = "Parameters", Transient, VisibleInstanceOnly)
	float MaxSpeed = 0.f;
	UPROPERTY(Category = "Parameters", Transient, VisibleInstanceOnly)
	float RateOfClimb = 0.f;
	UPROPERTY(Category = "Parameters", Transient, VisibleInstanceOnly)
	float MaxAltitude = 0.f;
	UPROPERTY(Category = "Parameters", Transient, VisibleInstanceOnly)
	float TurnTime = 0.f;
	UPROPERTY(Category = "Parameters", Transient, VisibleInstanceOnly)
	float RollTime = 0.f;
	UPROPERTY(Category = "Parameters", Transient, VisibleInstanceOnly)
	float YawTime = 0.f;
	UPROPERTY(Category = "Parameters", Transient, VisibleInstanceOnly)
	float TurnRadius = 0.f;

	UPROPERTY(Category = "Input", Transient, VisibleInstanceOnly)
	FVector InputControl = FVector::ZeroVector;
	UPROPERTY(Category = "Input", Transient, VisibleInstanceOnly)
	FVector ActualControl = FVector::ZeroVector;
	UPROPERTY(Category = "Input", Transient, VisibleInstanceOnly)
	float InputThrottle = 1.f;
	UPROPERTY(Category = "Input", Transient, VisibleInstanceOnly)
	float ActualThrottle = 1.f;

	UPROPERTY(Category = "State", Transient, VisibleInstanceOnly)
	float Speed = 0.f;
	UPROPERTY(Category = "State", Transient, VisibleInstanceOnly)
	float ForwardSpeed = 0.f;
	UPROPERTY(Category = "State", Transient, VisibleInstanceOnly)
	float Altitude = 0.f;
	UPROPERTY(Category = "State", Transient, VisibleInstanceOnly)
	bool Crashed = false;

public:
	
	UAircraftMovementComponent();

	virtual void InitializeComponent() override;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void TickComponent(float InDeltaTime, enum ELevelTick InTickType, FActorComponentTickFunction* InThisTickFunction) override;

	// BUSINESS LOGIC

	void ForceSpeed(float InSpeed);

	void SetCrashed();

	void SetInputHorizontal(float InValue);
	void SetInputVertical(float InValue);

	void DebugDraw();

private: 

	void UpdateParameters(float InDeltaTime);
	void UpdateSimulation(float InDeltaTime);
	
	void RegisterDebugDrawTick(bool bRegister);

	FAirctaftMovementComponentDebugDrawTickFunction DebugDrawTickFunction;
};

FORCEINLINE void UAircraftMovementComponent::SetInputHorizontal(float InValue)
{
	InputControl.Y = InValue;
}

FORCEINLINE void UAircraftMovementComponent::SetInputVertical(float InValue)
{
	InputControl.Z = InValue;
}