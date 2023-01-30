#include "stdafx.h"
#include "Collider.h"

Collider::Collider(Collider& col)
{
    m_collisionMask = col.m_collisionMask;
    m_curCollisions = col.m_curCollisions;
    m_collisions = col.m_collisions;
    m_isTrigger = col.m_isTrigger;
    m_lastValidPosition = col.m_lastValidPosition;
    m_layer = col.m_layer;
    m_transform = col.m_transform;
    m_type = col.m_type;
    m_blackList = col.m_blackList;
}

float Collider::Clamp(float min, float max, float value)
{
    if (value > max)
        return max;
    else if (value < min)
        return min;
    else
        return value;
}
template <typename T>
void Collider::RemoveDuplicateElements(std::vector<T>& vec)
{
    std::vector<int> duplicateIndices;
    //remove duplicates
    for (int i = 0; i < vec.size(); i++)
    {
        //find duplicates
        for (int n = 0; n < vec.size(); n++)
        {
            if (i == n)
                continue;

            if (vec[i] == vec[n])
                duplicateIndices.push_back(n);
        }

        //remove
        for (int n = 0; n < duplicateIndices.size(); n++)
        {
            vec.erase(vec.begin() + n);
        }
        duplicateIndices.clear();
    }
}


void Collider::OnEnter(Collider& col)
{
    for (auto& callback : m_onEnterCallbacks)
        callback(col);
}

void Collider::OnLeave(Collider& col)
{
    for (auto& callback : m_onLeaveCallbacks)
        callback(col);
}

void Collider::ManageCollisions()
{
    //check if entering or leaving
    std::vector<int> curintersectmatchesCount(m_curCollisions.size());
    for ( auto& [key, value] : m_collisions )
    {
        bool left = true;

        for (int n = 0; n < m_curCollisions.size(); n++)
        {
            //auto pastintersection = m_curintersections[n];
            if (m_curCollisions[n] == key)
            {
                left = false;
                curintersectmatchesCount[n]++;
                if (value == CollisionState::Entering || value == CollisionState::Leaving)
                {
                    value = CollisionState::Staying;
                }
            }
        }

        if (left)
        {
            value = CollisionState::Leaving;
        }
    }
    for (int i = 0; i < m_curCollisions.size(); i++)
    {
        if (curintersectmatchesCount[i] == 0)
        {
            m_collisions.emplace(m_curCollisions[i], CollisionState::Entering);
        }
    }
    m_curCollisions.clear();
}

void Collider::ProcessCollisions()
{
    if ( m_collisions.size() == 0 )
        return;

    //Run functions
    for ( std::map<std::shared_ptr<Collider>, CollisionState>::iterator itr = m_collisions.begin(); itr != m_collisions.end(); )
    {
        if ( itr->first )
        {
            switch ( itr->second )
            {
            case CollisionState::Entering:
                OnEnter(*itr->first);
                ++itr;
                break;
            case CollisionState::Leaving:
                OnLeave(*itr->first);
                if ( m_collisions.size() == 1 )
                    return;
                itr = m_collisions.erase( itr );
                break;
            }
        }
    }
}

void Collider::Update()
{
    if (m_curCollisions.size() == 0 && m_collisions.size() == 0)
        return;

    RemoveDuplicateElements<std::shared_ptr<Collider>>(m_curCollisions);
    ManageCollisions();
    ProcessCollisions();
}

bool Collider::CheckBlackList(std::shared_ptr<Collider> collider)
{
    if (m_blackList.size() == 0)
    {
        return false;
    }

    for (int i = 0; i < m_blackList.size(); i++)
    {
        if (m_blackList[i].get() == collider.get())
        {
            return true;
        }
    }
    return false;
}
