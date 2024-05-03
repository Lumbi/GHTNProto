// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GHTNBrainComponent.generated.h"

class GHTNOperationComponent;

struct UGHTNConditionPrivateImpl;
USTRUCT(BlueprintType) struct FGHTNCondition
{
	GENERATED_USTRUCT_BODY()

	FGHTNCondition();

	// FGHTNCondition(FGHTNCondition&) = default;
	// FGHTNCondition& operator=(FGHTNCondition&) = default;

private:
	friend class UGHTNBlueprintLibrary;
	friend class UGHTNBrainComponent;
	TSharedPtr<UGHTNConditionPrivateImpl> PrivateImpl;
};

UCLASS()
class UGHTNBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category = "GHTN|Condition")
	static FGHTNCondition Equal(
		UPARAM(DisplayName = "World Property") int32 property,
		UPARAM(DisplayName = "World State") int32 state,
		UPARAM(DisplayName = "Negate") bool negate = false
	);

	UFUNCTION(BlueprintPure, Category = "GHTN|Condition")
	static FGHTNCondition GreaterThan
	(
		UPARAM(DisplayName = "World Property") int32 property,
		UPARAM(DisplayName = "World State") int32 state
	);

	UFUNCTION(BlueprintPure, Category = "GHTN|Condition")
	static FGHTNCondition LessThan
	(
		UPARAM(DisplayName = "World Property") int32 property,
		UPARAM(DisplayName = "World State") int32 state
	);

	UFUNCTION(BlueprintPure, Category = "GHTN|Condition")
	static FGHTNCondition And(
		UPARAM(ref) FGHTNCondition& first,
		UPARAM(ref) FGHTNCondition& second
	);

	UFUNCTION(BlueprintPure, Category = "GHTN|Condition")
	static FGHTNCondition Or(
		UPARAM(ref) FGHTNCondition& first,
		UPARAM(ref) FGHTNCondition& second
	);
};

struct UGHTNBrainPrivateImpl;
struct UGHTNOperationExecutorPrivateImpl;
UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UGHTNBrainComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGHTNBrainComponent();

	virtual void InitializeComponent() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "GHTN|Task")
	int32 SingleTask(
		UPARAM(DisplayName = "Operation") UGHTNOperationComponent* operation
	);

	UFUNCTION(BlueprintCallable, Category = "GHTN|Task")
	int32 AllTasks(
		UPARAM(DisplayName = "Sub Tasks") TArray<int32> subtasks
	);

	UFUNCTION(BlueprintCallable, Category = "GHTN|Task")
	int32 AnyTasks(
		UPARAM(DisplayName = "Sub Tasks") TArray<int32> subtasks
	);

	UFUNCTION(BlueprintCallable, Category = "GHTN|Task")
	int32 SetConditions(
		UPARAM(DisplayName = "Task") int32 taskHandle,
		UPARAM(ref) FGHTNCondition& condition
	);

	UFUNCTION(BlueprintCallable, Category = "GHTN|Task")
	int32 SetParameter(
		UPARAM(DisplayName = "Task") int32 taskHandle,
		UPARAM(DisplayName = "Parameter") int32 parameter,
		UPARAM(DisplayName = "Value") int32 value
	);

	UFUNCTION(BlueprintCallable, Category = "GHTN|Task")
	int32 AddExpectation(
		UPARAM(DisplayName = "Task") int32 taskHandle,
		UPARAM(DisplayName = "World Property") int32 property,
		UPARAM(DisplayName = "Expected State") int32 state
	);

	UFUNCTION(BlueprintCallable, Category = "GHTN|Task")
	void SetRoot(
		UPARAM(DisplayName = "Task") int32 taskHandle
	);

public:
	void SetWorldProperty(int32 property, int32 value);

private:
	friend struct UGHTNOperationExecutorPrivateImpl;
	TSharedPtr<UGHTNBrainPrivateImpl> PrivateImpl;
};
