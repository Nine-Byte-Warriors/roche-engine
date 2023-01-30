#pragma once
#ifndef AIIDLE_H
#define AIIDLE_H

#include "AIState.h"
#include "Agent.h"

namespace AILogic
{
	class AIIdle : public AIState
	{
	public:
		AIIdle(Agent* pAgent) : AIState(pAgent) {};
		virtual ~AIIdle() = default;
		
		virtual void Update(const float dt) override;
		virtual void Enter() override {};
		virtual void Exit() override {};

		//virtual float CalculateActivation() override {};
	};
}
#endif // !AIIDLE_H
