#include "Aircraft.h"

#include "AircraftMovementComponent.h"
#include "AircraftTelemetryComponent.h"

#include "Components/BoxComponent.h"

#if WITH_EDITORONLY_DATA
#include "Components/ArrowComponent.h"
#endif // WITH_EDITORONLY_DATA

#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"

#include "GameFramework/SpringArmComponent.h"

#include "Camera/CameraComponent.h"

#include "DisplayDebugHelpers.h"

AAircraft::AAircraft()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = ETickingGroup::TG_PrePhysics;

	UBoxComponent* BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Shape"));
	BoxComponent->InitBoxExtent(FVector(500.f, 100.f, 100.f));
	BoxComponent->SetCollisionProfileName(TEXT("Aircraft"));
	BoxComponent->SetNotifyRigidBodyCollision(true);
	BoxComponent->SetGenerateOverlapEvents(false);
	BoxComponent->SetCanEverAffectNavigation(false);
	BoxComponent->CanCharacterStepUpOn = ECB_No;
	BoxComponent->bApplyImpulseOnDamage = false;

	ShapeComponent = BoxComponent;

	RootComponent = BoxComponent;

#if WITH_EDITORONLY_DATA

	ArrowComponent = CreateEditorOnlyDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	
	if (ArrowComponent != nullptr)
	{
		ArrowComponent->ArrowColor = FColor(0, 255, 0);
		ArrowComponent->ArrowSize = 2.f;
		ArrowComponent->ArrowLength = 350.f;

		ArrowComponent->SetupAttachment(ShapeComponent);
	}

#endif // WITH_EDITORONLY_DATA

	MeshComponent = CreateOptionalDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	
	if (MeshComponent != nullptr)
	{
		MeshComponent->PrimaryComponentTick.TickGroup = TG_PrePhysics;

		static FName MeshCollisionProfileName(TEXT("NoCollision"));

		MeshComponent->SetCollisionProfileName(MeshCollisionProfileName);
		MeshComponent->SetNotifyRigidBodyCollision(false);
		MeshComponent->SetGenerateOverlapEvents(false);
		MeshComponent->SetCanEverAffectNavigation(false);
		MeshComponent->CanCharacterStepUpOn = ECB_No;
		MeshComponent->AlwaysLoadOnClient = true;
		MeshComponent->AlwaysLoadOnServer = true;
		MeshComponent->bOwnerNoSee = false;
		MeshComponent->bCastDynamicShadow = true;
		MeshComponent->bAffectDynamicIndirectLighting = true;
		MeshComponent->bApplyImpulseOnDamage = false;

		MeshComponent->SetupAttachment(ShapeComponent);
	}

	SpringArmComponent = CreateOptionalDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	
	if (SpringArmComponent != nullptr)
	{
		SpringArmComponent->TargetArmLength = 1500.f;
		SpringArmComponent->SocketOffset = FVector(0.f, 0.f, 200.f);
		SpringArmComponent->bDoCollisionTest = false;
		SpringArmComponent->bEnableCameraRotationLag = true;
		SpringArmComponent->CameraRotationLagSpeed = 2.f;
		SpringArmComponent->bInheritPitch = true;
		SpringArmComponent->bInheritYaw = true;
		SpringArmComponent->bInheritRoll = false;

		SpringArmComponent->SetupAttachment(ShapeComponent);
	}

	CameraComponent = CreateOptionalDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	
	if (CameraComponent != nullptr)
	{
		CameraComponent->SetupAttachment(SpringArmComponent);
	}

	AircraftMovementComponent = CreateDefaultSubobject<UAircraftMovementComponent>(TEXT("AircraftMovement"));
	AircraftTelemetryComponent = CreateDefaultSubobject<UAircraftTelemetryComponent>(TEXT("AircraftTelemetry"));
}

void AAircraft::BeginPlay()
{
	Super::BeginPlay();

	if (AircraftMovementComponent != nullptr)
	{
		AircraftMovementComponent->ForceSpeed(0.8f);
	}
}

void AAircraft::NotifyHit(UPrimitiveComponent* InComponent, AActor* InOtherActor, UPrimitiveComponent* InOtherComponent, bool bInSelfMoved, FVector InHitLocation, FVector InHitNormal, FVector InNormalImpulse, const FHitResult& InHit)
{
	Super::NotifyHit(InComponent, InOtherActor, InOtherComponent, bInSelfMoved, InHitLocation, InHitNormal, InNormalImpulse, InHit);

	if (AircraftMovementComponent != nullptr)
	{
		AircraftMovementComponent->SetCrashed();
	}
}

void AAircraft::SetupPlayerInputComponent(UInputComponent* InPlayerInputComponent)
{
	Super::SetupPlayerInputComponent(InPlayerInputComponent);

	if (InPlayerInputComponent == nullptr)
	{
		return;
	}

	InPlayerInputComponent->BindAxis("Horizontal", this, &AAircraft::SetInputHorizontal);
	InPlayerInputComponent->BindAxis("Vertical", this, &AAircraft::SetInputVertical);
}

void AAircraft::DisplayDebug(UCanvas* InCanvas, const FDebugDisplayInfo& InDebugDisplay, float& InYL, float& InYPos)
{
	Super::DisplayDebug(InCanvas, InDebugDisplay, InYL, InYPos);

	static FName NAME_Telemetry = FName(TEXT("TELEMETRY"));

	if (InDebugDisplay.IsDisplayOn(NAME_Telemetry))
	{
		if (AircraftTelemetryComponent != nullptr)
		{
			AircraftTelemetryComponent->DrawTelemetry(InCanvas, InYL, InYPos);
		}
	}
}

void AAircraft::SetInputHorizontal(float InValue)
{
	if (AircraftMovementComponent != nullptr)
	{
		AircraftMovementComponent->SetInputHorizontal(InValue);
	}
}

void AAircraft::SetInputVertical(float InValue)
{
	if (AircraftMovementComponent != nullptr)
	{
		AircraftMovementComponent->SetInputVertical(InValue);
	}
}