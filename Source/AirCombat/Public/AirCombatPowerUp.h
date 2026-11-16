#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AirCombatPowerUp.generated.h"

UCLASS()
class AIRCOMBAT_API AAirCombatPowerUp : public AActor
{
	GENERATED_BODY()
	
private:

	UPROPERTY(EditAnywhere, Category = "Power Up")
	FString		TargetProperty = "";
	UPROPERTY(EditAnywhere, Category = "Power Up")
	float		Multiplier = 1.f;

public:	

	AAirCombatPowerUp();

	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

private:

	UFUNCTION()
	void OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
};