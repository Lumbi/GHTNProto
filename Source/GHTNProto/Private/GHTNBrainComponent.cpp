// Fill out your copyright notice in the Description page of Project Settings.


#include "GHTNBrainComponent.h"

#include "GHTNOperationComponent.h"
#include "GHTN/Planner.h"
#include "GHTN/Domain.h"
#include "GHTN/Task.h"
#include "GHTN/World.h"
#include "GHTN/Operation.h"
#include "GHTN/Condition.h"
#include "GHTN/Runner.h"

struct UGHTNConditionPrivateImpl
{
	UGHTNConditionPrivateImpl()
		: builder(GHTN::ConditionTreeBuilder::Just(GHTN::Condition(0, GHTN::Predicate::equal, 0)))
	{
	}

	GHTN::ConditionTreeBuilder::NodeBuilder builder;
};

FGHTNCondition::FGHTNCondition()
{
	PrivateImpl = MakeShared<UGHTNConditionPrivateImpl>();
}

FGHTNCondition UGHTNBlueprintLibrary::Equal(int32 property, int32 state, bool negate)
{
	FGHTNCondition condition;
	condition.PrivateImpl->builder = GHTN::ConditionTreeBuilder::Just(GHTN::Condition(property, GHTN::Predicate::equal, state, negate));
	return condition;
}

FGHTNCondition UGHTNBlueprintLibrary::GreaterThan(int32 property, int32 state)
{
	FGHTNCondition condition;
	condition.PrivateImpl->builder = GHTN::ConditionTreeBuilder::Just(GHTN::Condition(property, GHTN::Predicate::greater, state));
	return condition;
}

FGHTNCondition UGHTNBlueprintLibrary::LessThan(int32 property, int32 state)
{
	FGHTNCondition condition;
	condition.PrivateImpl->builder = GHTN::ConditionTreeBuilder::Just(GHTN::Condition(property, GHTN::Predicate::less, state));
	return condition;
}

FGHTNCondition UGHTNBlueprintLibrary::And(FGHTNCondition& first, FGHTNCondition& second)
{
	FGHTNCondition condition;
	condition.PrivateImpl->builder = GHTN::ConditionTreeBuilder::And(GHTN::ConditionTreeBuilder::NodeBuilder(first.PrivateImpl->builder), GHTN::ConditionTreeBuilder::NodeBuilder(second.PrivateImpl->builder));
	return condition;
}

FGHTNCondition UGHTNBlueprintLibrary::Or(FGHTNCondition& first, FGHTNCondition& second)
{
	FGHTNCondition condition;
	condition.PrivateImpl->builder = GHTN::ConditionTreeBuilder::Or(GHTN::ConditionTreeBuilder::NodeBuilder(first.PrivateImpl->builder), GHTN::ConditionTreeBuilder::NodeBuilder(second.PrivateImpl->builder));
	return condition;
}

struct UGHTNBrainPrivateImpl
{
	static constexpr int32 InvalidTaskHandle = -1;
	static constexpr int32 InvalidOperationHandle = -1;

	UGHTNBrainPrivateImpl() = default;

	GHTN::World World;
	TUniquePtr<GHTN::Domain> Domain;
	TArray<TUniquePtr<GHTN::Task>> Tasks;
	TArray<TUniquePtr<GHTN::Operation>> Operations;
	TMap<GHTN::Operation*, UGHTNOperationComponent*> OperationsMapping;
	GHTN::Plan Plan;
	TUniquePtr<GHTN::Runner> Runner;
	TUniquePtr<UGHTNOperationExecutorPrivateImpl> Executor;

	GHTN::Task* GetTask(int32 taskHandle) const
	{
		if (taskHandle >= 0 && taskHandle < Tasks.Num())
		{
			return Tasks[taskHandle].Get();
		}
		UE_LOG(LogTemp, Error, TEXT("Task with handle '%d' does not exist."), taskHandle);
		return nullptr;
	}
};

struct UGHTNOperationExecutorPrivateImpl : public GHTN::OperationExecutorInterface
{
	UGHTNOperationExecutorPrivateImpl(UGHTNBrainComponent* brainComponent)
		: BrainComponent(brainComponent) 
	{};

	void Execute(GHTN::Operation const& operation, Parameters parameters) override
	{
		TMap<uint8, uint8> parameterMap;
		for (uint8 i = 0; i < static_cast<uint8>(GHTN::Parameter::MAX_COUNT); ++i)
		{
			parameterMap.Add(i, parameters[i]);
		}

		if (UGHTNOperationComponent** operationComponent = BrainComponent->PrivateImpl->OperationsMapping.Find(&operation))
		{
			(*operationComponent)->StartExecution(parameterMap);
		}
	}

	GHTN::Operation::Result Check(GHTN::Operation const& operation) override
	{
		if (UGHTNOperationComponent** operationComponent = BrainComponent->PrivateImpl->OperationsMapping.Find(&operation))
		{
			switch ((*operationComponent)->Result)
			{
			case EGHTNOperationResult::RUNNING: return GHTN::Operation::Result::running;
			case EGHTNOperationResult::FAILURE: return GHTN::Operation::Result::failure;
			case EGHTNOperationResult::SUCCESS: return GHTN::Operation::Result::success;
			}
		}
		UE_LOG(LogTemp, Error, TEXT("Executor could not execute operation because it does not exist."));
		return GHTN::Operation::Result::failure;
	}

	void Stop(GHTN::Operation const& operation) override
	{
		if (UGHTNOperationComponent** operationComponent = BrainComponent->PrivateImpl->OperationsMapping.Find(&operation))
		{
			(*operationComponent)->StopExecution();
		}
	}

private:
	UGHTNBrainComponent* BrainComponent = nullptr;
};

// Sets default values for this component's properties
UGHTNBrainComponent::UGHTNBrainComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;

	PrivateImpl = MakeShared<UGHTNBrainPrivateImpl>();
}

void UGHTNBrainComponent::InitializeComponent()
{
	Super::InitializeComponent();

	// Init Operations Bindings
	TInlineComponentArray<UGHTNOperationComponent*> OperationComponents(GetOwner());
	for (UGHTNOperationComponent* OperationComponent : OperationComponents)
	{
		OperationComponent->OperationHandle = PrivateImpl->Operations.Emplace(MakeUnique<GHTN::Operation>());
		PrivateImpl->OperationsMapping.Add(PrivateImpl->Operations.Last().Get()) = OperationComponent;
	}

	// Init Runner & Bind Executor
	PrivateImpl->Executor = MakeUnique<UGHTNOperationExecutorPrivateImpl>(this);
	PrivateImpl->Runner = MakeUnique<GHTN::Runner>(PrivateImpl->Executor.Get());
}

// Called every frame
void UGHTNBrainComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PrivateImpl->Runner && PrivateImpl->Domain)
	{
		if (!PrivateImpl->Runner->IsRunning())
		{
			PrivateImpl->Plan = GHTN::Planner::Find(*PrivateImpl->Domain.Get(), PrivateImpl->World);
			PrivateImpl->Runner->Run(&PrivateImpl->Plan);
		}
		else
		{
			PrivateImpl->Runner->Update(PrivateImpl->World);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Runner or Domain not initialized."));
	}
}

int32 UGHTNBrainComponent::SingleTask(UGHTNOperationComponent* operationComponent)
{
	int32 OperationHandle = operationComponent->OperationHandle;
	if (OperationHandle >= 0 && OperationHandle < PrivateImpl->Operations.Num())
	{
		if (GHTN::Operation* operation = PrivateImpl->Operations[OperationHandle].Get())
		{
			return PrivateImpl->Tasks.Emplace(MakeUnique<GHTN::Task>(operation));
		}
	}
	UE_LOG(LogTemp, Error, TEXT("Could not create 'SINGLE' task"));
	return UGHTNBrainPrivateImpl::InvalidTaskHandle;
}

int32 UGHTNBrainComponent::AllTasks(TArray<int32> subtasksHandles)
{
	TUniquePtr<GHTN::Task> composite = MakeUnique<GHTN::Task>(GHTN::Task::ALL);
	for (int32 subtaskHandle : subtasksHandles)
	{
		GHTN::Task const* task = PrivateImpl->GetTask(subtaskHandle);
		if (task)
		{
			composite->AddSubTask(task);
		}
	}
	
	return PrivateImpl->Tasks.Emplace(MoveTemp(composite));

	/*
	if (!composite->GetSubTasks()->empty())
	{
		return PrivateImpl->Tasks.Emplace(MoveTemp(composite));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Could not create 'ALL' task without subtasks"));
		return UGHTNBrainPrivateImpl::InvalidTaskHandle;
	}
	*/
}

int32 UGHTNBrainComponent::AnyTasks(TArray<int32> subtasksHandles)
{
	TUniquePtr<GHTN::Task> composite = MakeUnique<GHTN::Task>(GHTN::Task::ANY);
	for (int32 subtaskHandle : subtasksHandles)
	{
		GHTN::Task const* task = PrivateImpl->GetTask(subtaskHandle);
		if (task)
		{
			composite->AddSubTask(task);
		}
	}

	return PrivateImpl->Tasks.Emplace(MoveTemp(composite));

	/*
	if (!composite->GetSubTasks()->empty())
	{
		return PrivateImpl->Tasks.Emplace(MoveTemp(composite));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Could not create 'ALL' task without subtasks"));
		return UGHTNBrainPrivateImpl::InvalidTaskHandle;
	}
	*/
}

int32 UGHTNBrainComponent::SetConditions(int32 taskHandle, FGHTNCondition& condition)
{
	if (GHTN::Task* task = PrivateImpl->GetTask(taskHandle))
	{
		task->SetConditions(GHTN::ConditionTree(condition.PrivateImpl->builder));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Could not set conditions on task with handle '%d'."), taskHandle);
	}
	return taskHandle;
}

int32 UGHTNBrainComponent::SetParameter(int32 taskHandle, int32 parameter, int32 value)
{
	if (GHTN::Task* task = PrivateImpl->GetTask(taskHandle))
	{
		task->SetParameter(static_cast<GHTN::Parameter::Index>(parameter), static_cast<GHTN::Parameter::Value>(value));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Could not set conditions on task with handle '%d'."), taskHandle);
	}
	return taskHandle;
}

int32 UGHTNBrainComponent::AddExpectation(int32 taskHandle, int32 property, int32 state)
{
	if (GHTN::Task* task = PrivateImpl->GetTask(taskHandle))
	{
		task->AddExpectation(static_cast<GHTN::World::Property>(property), static_cast<GHTN::World::State>(state));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Could not set conditions on task with handle '%d'."), taskHandle);
	}
	return taskHandle;
}

void UGHTNBrainComponent::SetRoot(int32 taskHandle)
{
	if (taskHandle >= 0 && taskHandle < PrivateImpl->Tasks.Num())
	{
		GHTN::Task* task = PrivateImpl->Tasks[taskHandle].Get();
		PrivateImpl->Domain = MakeUnique<GHTN::Domain>(task);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot set root on non-existant task with handle '%d'."), taskHandle);
		PrivateImpl->Domain.Reset();
	}
}

void UGHTNBrainComponent::SetWorldProperty(int32 property, int32 value)
{
	PrivateImpl->World.Set(static_cast<GHTN::World::Property>(property), static_cast<GHTN::World::State>(value));
}
