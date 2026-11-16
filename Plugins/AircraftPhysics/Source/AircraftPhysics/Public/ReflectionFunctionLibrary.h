

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ReflectionFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class AIRCRAFTPHYSICS_API UReflectionFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	static FProperty* RetriveProperty(UObject* InObject, const FString& InPath, void*& OutTarget);
};
