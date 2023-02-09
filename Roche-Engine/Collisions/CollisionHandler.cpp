#include "stdafx.h"
#include "CollisionHandler.h"

CollisionHandler::CollisionHandler(CollisionMatrix collisionMatrix)
{
    m_collisionMatrix.SetMatrix(collisionMatrix);
}

void CollisionHandler::AddCollider(std::shared_ptr<Collider> collider)
{
    m_colliders.push_back(collider);
}

void CollisionHandler::AddCollider(std::vector<std::shared_ptr<Collider>> colliders)
{
    for (int i = 0; i < colliders.size(); i++)
    {
        m_colliders.push_back(colliders[i]);
    }
}

void CollisionHandler::SetMatrix( 
    bool dD, bool dP, bool dE, bool dPPj, bool dEPj,
    bool pD, bool pP, bool pE, bool pPPj, bool pEPj,
    bool eD, bool eP, bool eE, bool ePPj, bool eEPj,
    bool ppjD, bool ppjP, bool ppjE, bool ppjPPj, bool ppjEPj,
    bool epjD, bool epjP, bool epjE, bool epjPPj, bool epjEPj)
{

    m_collisionMatrix.SetMatrix(
        dD, dP, dE, dPPj, dEPj,
        pD, pP, pE, pPPj, pEPj,
        eD, eP, eE, ePPj, eEPj,
        ppjD, ppjP, ppjE, ppjPPj, ppjEPj,
        epjD, epjP, epjE, epjPPj, epjEPj);
}

void CollisionHandler::RemoveAllColliders()
{
    m_colliders.clear();
}
void CollisionHandler::RemoveCollider(std::shared_ptr<Collider>collider)
{
    for (int i = 0; i < m_colliders.size(); i++)
    {
        if (m_colliders[i].get() == collider.get())
        {
            m_colliders.erase(m_colliders.begin() + i);
            return;
        }
    }
}

void CollisionHandler::CollisionCheckAll()
{
    int startIndex = 0;
    bool isCollision;
    bool layersInteract;
    bool collider1Interacts;
    bool collider2Interacts;
    for (int i = 0; i < m_colliders.size(); i++)
    {
        if (m_colliders[i]->GetIsEnabled() == false)
        {
            continue;
        }
        isCollision = false;
        for (int n = 0; n < m_colliders.size(); n++)
        {
            layersInteract = m_collisionMatrix.GetElement((int)m_colliders[i]->GetLayer(), (int)m_colliders[n]->GetLayer());
            collider1Interacts = m_colliders[i]->GetCollisionMask().m_mask[(int)m_colliders[n]->GetLayer()];
            collider2Interacts = m_colliders[n]->GetCollisionMask().m_mask[(int)m_colliders[i]->GetLayer()];

            if (n == i || layersInteract == false || collider1Interacts == false|| collider2Interacts == false ||m_colliders[n]->GetIsEnabled() ==false)
            {
                continue;
            }

            bool isIntersection = m_colliders[i]->CollisionCheck(m_colliders[n]);

            if (isIntersection)
            {
                isCollision = true;

                m_colliders[i]->LogCollision(m_colliders[n]);
                m_colliders[n]->LogCollision(m_colliders[i]);

                if (m_colliders[i]->GetIsStatic() == false)
                {
                    m_colliders[i]->Resolution(m_colliders[n]);
                }
            }
        }
        if (!isCollision)
        {
            m_colliders[i]->UpdateLastValidPosition();
        }
    }
}

void CollisionHandler::Update()
{
    CollisionCheckAll();
    for (auto& col : m_colliders)
    {
        col->Update();
    }
}
