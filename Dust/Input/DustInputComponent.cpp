#include "Input/DustInputComponent.h"

UDustInputComponent::UDustInputComponent(const FObjectInitializer& ObjectInitializer)
{
}

void UDustInputComponent::RemoveBinds(TArray<uint32>& BindHandles)
{
	for (uint32 Handle : BindHandles)
	{
		RemoveBindingByHandle(Handle);
	}
	BindHandles.Reset();
}
