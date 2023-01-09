#pragma once
#ifndef AIFLEE_H
#define AIFLEE_H

#include "AIState.h"
#include "Agent.h"

namespace AILogic
{
    
    class AIFlee : public AIState
    {
    public:
		AIFlee(Agent* pAgent) : AIState(pAgent) {};
		virtual ~AIFlee() = default;

		virtual void Update(const float dt) override;
        virtual void SetTarget(Agent* pTarget) { m_pTarget = pTarget; }
        
    private:
		Agent* m_pTarget;
    };

}

#endif // !AIFLEE_H