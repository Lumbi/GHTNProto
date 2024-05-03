// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GHTNOperationComponent.generated.h"

UENUM(BlueprintType)
enum class EGHTNOperationResult : uint8
{
	RUNNING		UMETA(DisplayName = "Running"),
	FAILURE		UMETA(DisplayName = "Failure"),
	SUCCESS		UMETA(DisplayName = "Success")
};

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UGHTNOperationComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGHTNOperationComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "GHTN|Operation")
	void StartExecution(
		UPARAM(Ref, DisplayName = "Parameters") TMap<uint8, uint8> const& parameters
	);

	UFUNCTION(BlueprintImplementableEvent, Category = "GHTN|Operation")
	void StopExecution();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GHTN|Operation")
	int32 OperationHandle;

	UPROPERTY(BlueprintReadWrite, Category = "GHTN|Operation")
	EGHTNOperationResult Result;
};
