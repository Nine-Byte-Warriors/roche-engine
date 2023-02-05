#pragma once
#ifndef AIFIRE_H
#define AIFIRE_H

#include "AIState.h"
#include "Agent.h"
#include "Emitter.h"

namespace AILogic
{
	class AIFire : public AIState
	{
	public:
		AIFire(Agent* pAgent) :
			AIState(pAgent), 
			m_pEmitter(nullptr),
			m_fSensingRange(100.0f),
			m_fFireRate(2.0f),
			m_fFireTimer(0.0f)
		{};
		virtual ~AIFire() = default;

		virtual void Update(const float dt) override;
		virtual void Enter() override;
		//virtual void Exit() override {};

	private:
		void GetFireParams();

		float m_fSensingRange;
		float m_fFireRate;
		float m_fFireTimer;
		
		std::shared_ptr<Emitter> m_pEmitter;
	};
}

#endif // !AIFIRE_H
