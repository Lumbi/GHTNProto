#pragma once

#include "GHTN.h"

#include "Planner.h"
#include "Operation.h"
#include "Parameter.h"

namespace GHTN
{
	class OperationExecutorInterface
	{
	public:
		using Parameters = std::span<const Parameter::Value, Parameter::MAX_COUNT>;

		virtual ~OperationExecutorInterface() {};

	public:
		virtual void Execute(Operation const&, Parameters) = 0;

		virtual Operation::Result Check(Operation const&) = 0;

		virtual void Stop(Operation const&) = 0;
	};

	class Runner
	{
	public:
		GHTN_API Runner(OperationExecutorInterface*);

		GHTN_API void Run(Plan const*);

		GHTN_API void Abort();

		GHTN_API bool IsRunning() const;

		GHTN_API void Update(World&);

	private:
		void AdvanceToNextTask(World&);

	private:
		Plan const* m_Plan = nullptr;
		Plan::const_iterator m_CurrentTask;
		Operation const* m_CurrentOperation = nullptr;
		OperationExecutorInterface* m_OperationExecutor = nullptr;
	};
}
