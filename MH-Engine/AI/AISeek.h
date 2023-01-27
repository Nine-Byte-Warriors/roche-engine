#pragma once
#ifndef AISEEK_H
#define AISEEK_H

#include "AIState.h"
#include "Entity.h"
#include "Agent.h"

namespace AILogic
{
	class AISeek : public AIState
	{
	public:
		AISeek(Agent* pAgent) : AIState(pAgent) {};
		virtual ~AISeek() = default;

		virtual void Update(const float dt) override;
		virtual void Enter() override {};
		virtual void Exit() override {};

		//virtual float CalculateActivation() override {};
		inline void SetTarget(Entity* pTarget) { m_pTarget = pTarget; }

	private:
		Entity* m_pTarget;
	};
}

#endif // AISEEK_H
