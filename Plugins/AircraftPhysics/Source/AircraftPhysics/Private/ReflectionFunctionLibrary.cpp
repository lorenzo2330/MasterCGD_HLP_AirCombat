#include "ReflectionFunctionLibrary.h"

FProperty* UReflectionFunctionLibrary::RetriveProperty(UObject* InObject, const FString& InPath, void*& OutTarget)
{
	// Example:

	//		AircraftMovement.Speed
	//		AircraftMovement.ForwardSpeed

	ensureAlways(InObject);

	if (InObject == nullptr)
	{
		return nullptr;
	}

	FString ObjectString;
	FString PropertyString;
	bool bSucceded = InPath.Split(".", &ObjectString, &PropertyString);

	ensureAlways(bSucceded);

	UObject* TargetObject = nullptr;
	if (ObjectString.Equals("This", ESearchCase::IgnoreCase))
	{
		TargetObject = InObject;
	}
	else
	{
		AActor* Actor = Cast<AActor>(InObject);

		ensureAlways(Actor);

		if (Actor)
		{
			TArray<UActorComponent*> ActorComponents;
			Actor->GetComponents(ActorComponents);

			for (UActorComponent* Comp : ActorComponents)
			{
				if (ObjectString.Equals(Comp->GetName(), ESearchCase::IgnoreCase))
				{
					TargetObject = Comp;
					break;
				}
			}
		}
	}

	ensureAlways(TargetObject);

	OutTarget = TargetObject;

	FProperty* OutProperty = nullptr;

	if (TargetObject)
	{
		UClass* Class = TargetObject->GetClass();
		OutProperty = Class->FindPropertyByName(FName(*PropertyString));
	}

	return OutProperty;
}


