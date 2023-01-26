#include "stdafx.h"
#include "Collider.h"

Collider::Collider(Collider& col)
{
    m_collisionMask = col.m_collisionMask;
    m_curintersections = col.m_curintersections;
    m_intersections = col.m_intersections;
    m_isTrigger = col.m_isTrigger;
    m_lastValidPosition = col.m_lastValidPosition;
    m_layer = col.m_layer;
    m_tf = col.m_tf;
    m_type = col.m_type;
}

float Collider::Clamp(float min, float max, float value)
{
    if (value > max)
    {
        return max;
    }
    else if (value < min)
    {
        return min;
    }
    else
    {
        return value;
    }
}

void Collider::OnEnter(Collider& col)
{
    for (auto& callback : m_onEnterCallbacks)
    {
        callback(col);
    }
}

void Collider::OnExit(Collider& col)
{
    for (auto& callback : m_onExitCallbacks)
    {
        callback(col);
    }
}

template <typename T>
std::vector <T> Collider::RemoveDuplicateEntries(std::vector<T> vec)
{
    std::vector<int> duplicateIndices;
    //remove duplicates
    for (int i = 0; i < vec.size(); i++)
    {
        //find duplicates
        for (int n = 0; n < vec.size(); n++)
        {
            if (i == n) {
                continue;
            }
            if (vec[i] == vec[n])
            {
                duplicateIndices.push_back(n);
            }
        }

        //remove
        for (int n = 0; n < duplicateIndices.size(); n++)
        {
            vec.erase(vec.begin() + n);
        }
        duplicateIndices.clear();
    }
    return vec;
}
void Collider::ManageIntersections()
{
    //check if entering or leaving
    std::vector<int> curintersectmatchesCount(m_curintersections.size());
    for ( auto& [key, value] : m_intersections )
    {
        bool left = true;

        for (int n = 0; n < m_curintersections.size(); n++)
        {
            //auto pastintersection = m_curintersections[n];
            if (m_curintersections[n] == key)
            {
                left = false;
                curintersectmatchesCount[n]++;
                if (value == CollisionState::Entering || value == CollisionState::Leaving)
                {
                    value = CollisionState::Staying;
                }
            }
        }
        if (left == true)
        {
            value = CollisionState::Leaving;
        }
    }
    for (int i = 0; i < m_curintersections.size(); i++)
    {
        if (curintersectmatchesCount[i] == 0)
        {
            m_intersections.emplace(m_curintersections[i], CollisionState::Entering);
        }
    }
    m_curintersections.clear();
}
void Collider::Process()
{
    if ( m_intersections.size() < 1 )
        return;

    //Run functions
    for ( std::map<std::shared_ptr<Collider>, CollisionState>::iterator itr = m_intersections.begin(); itr != m_intersections.end(); )
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
                OnExit(*itr->first);
                itr = m_intersections.erase( itr );
                break;
            }
        }
    }
};
void Collider::Update()
{
    if (m_curintersections.size() == 0 && m_intersections.size() == 0)
    {
        return;
    }

    m_curintersections = RemoveDuplicateEntries<std::shared_ptr<Collider>>(m_curintersections);

    ManageIntersections();
    Process();
};