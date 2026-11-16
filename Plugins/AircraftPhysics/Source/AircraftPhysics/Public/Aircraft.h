#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Aircraft.generated.h"

class UShapeComponent;

#if WITH_EDITORONLY_DATA
class UArrowComponent;
#endif // WITH_EDITORONLY_DATA

class UStaticMeshComponent;
class USpringArmComponent;
class UCameraComponent;

class UInputComponent;

class UAircraftMovementComponent;
class UAircraftTelemetryComponent;

UCLASS()
class AIRCRAFTPHYSICS_API AAircraft : public APawn
{
	GENERATED_BODY()

public:

	AAircraft();

	virtual void SetupPlayerInputComponent(UInputComponent* InPlayerInputComponent) override;

	virtual void BeginPlay() override;
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	virtual void DisplayDebug(UCanvas* InCanvas, const FDebugDisplayInfo& InDebugDisplay, float& InYL, float& InYPos) override;

private:

	UPROPERTY(Category = Aircraft, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UShapeComponent> ShapeComponent;

#if WITH_EDITORONLY_DATA

	UPROPERTY()
	TObjectPtr<UArrowComponent> ArrowComponent;

#endif // WITH_EDITORONLY_DATA

	UPROPERTY(Category = Aircraft, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> MeshComponent;
	UPROPERTY(Category = Aircraft, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	UPROPERTY(Category = Aircraft, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(Category = Aircraft, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAircraftMovementComponent> AircraftMovementComponent;
	UPROPERTY(Category = Aircraft, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAircraftTelemetryComponent> AircraftTelemetryComponent;

	UFUNCTION(BlueprintCallable)
	void SetInputHorizontal(float InValue);
	UFUNCTION(BlueprintCallable)
	void SetInputVertical(float InValue);
};