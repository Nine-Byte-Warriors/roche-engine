#pragma once
#ifndef AIFOLLOW_H
#define AIFOLLOW_H

#include "AIState.h"
#include "Agent.h"

namespace AILogic
{
    class AIFollow : public AIState
    {
    public:
        AIFollow(Agent* pAgent) : AIState(pAgent) {};
        virtual ~AIFollow() = default;

        virtual void Update(const float dt) override;
        virtual void Enter() override { GetFollowParams(); };
        virtual void Exit() override {};

    private:
        void GetFollowParams();

        bool IsTargetInRange(Vector2f vAgentPos, Vector2f vTargetPos) noexcept 
            { return m_fFollowRange >= vAgentPos.Distance(vTargetPos); }

        float m_fFollowRange;
        float m_fRepulseRange;
        bool m_bKeepRange;
    };
}
#endif // !AIFOLLOW_H