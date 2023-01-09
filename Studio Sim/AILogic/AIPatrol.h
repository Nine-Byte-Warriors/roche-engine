#pragma once

#ifndef AIPATROL_H
#define AIPATROL_H

#include "AIState.h"
#include "Agent.h"

namespace AILogic
{
	class AIPatrol : public AIState
	{
	public:
		AIPatrol(Agent* pAgent) : AIState(pAgent) {};
		virtual ~AIPatrol() = default;

		virtual void Update(const float dt) override;
		virtual void Enter() override;
		virtual void Exit() override;

		virtual float CalculateActivation() override;

	};
}

#endif // !AIPATROL_H