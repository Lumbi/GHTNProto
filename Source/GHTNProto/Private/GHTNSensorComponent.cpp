#include "GHTNSensorComponent.h"

#include "GHTNBrainComponent.h"

// Sets default values for this component's properties
UGHTNSensorComponent::UGHTNSensorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
}

void UGHTNSensorComponent::InitializeComponent()
{
	BrainComponent = GetOwner()->FindComponentByClass<UGHTNBrainComponent>();
}

void UGHTNSensorComponent::SetWorldProperty(int32 property, int32 state)
{
	if (BrainComponent)
	{
		BrainComponent->SetWorldProperty(property, state);
	}
}
