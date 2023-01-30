#pragma once
#ifndef AIWANDER_H
#define AIWANDER_H

#include "AIState.h"
#include "Agent.h"
#include "RND.h"

namespace AILogic
{
    class AIWander : public AIState
    {
    public:
        AIWander(Agent* pAgent) : AIState(pAgent) {};
        virtual ~AIWander() = default;

        virtual void Update(const float dt) override;
        virtual void Enter() override { GetWanderParams(); }
        virtual void Exit() override {};

    private:
        void GetWanderParams();
        bool DelayCount(const float dt);

        float m_fWanderAngle;
        float m_fMaxDelay;
        float m_fDelayCount;
    };
}
#endif // !AIWANDER_H