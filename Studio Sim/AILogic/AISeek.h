#pragma once
#ifndef AISEEK_H
#define AISEEK_H

#include "AIState.h"
#include "Agent.h"

namespace AILogic
{
	class AISeek : public AIState
	{
	public:
		AISeek(Agent* pAgent) : AIState(pAgent) {};
		virtual ~AISeek() = default;

		virtual void Update(const float dt) override;
		//virtual void Enter() override {};
		//virtual void Exit() override {};

		//virtual float CalculateActivation() override {};

		virtual void SetTarget(Agent* pTarget) { m_pTarget = pTarget; }

	private:
		Agent* m_pTarget;
		//float m_fSpeed;
	};
}

#endif // AISEEK_H
