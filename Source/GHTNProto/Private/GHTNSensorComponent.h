// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "GHTNSensorComponent.generated.h"

class UGHTNBrainComponent;

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UGHTNSensorComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGHTNSensorComponent();

	virtual void InitializeComponent() override;

	UFUNCTION(BlueprintCallable, Category = "GHTN|Sensor")
	void SetWorldProperty(int32 property, int32 state);

private:
	UGHTNBrainComponent* BrainComponent = nullptr;
};
