#pragma once
#ifndef COLLISIONHANDLER_H
#define COLLISIONHANDLER_H

#include "Collision.h"
#include "CollisionMatrix.h"

//Monitors collisions between colliders and resolve them
class CollisionHandler
{
public:
    CollisionHandler() = default;
    CollisionHandler(CollisionMatrix collisionMatrix);

private:
    std::vector<std::shared_ptr<Collider>> m_colliders;
    CollisionMatrix m_collisionMatrix;
public:
    //Specify which colliders to monitor

    void AddCollider(std::shared_ptr<Collider> collider);
    void AddCollider(std::vector<std::shared_ptr<Collider>> colliders);

    void SetMatrix(CollisionMatrix collisionMatrix) { m_collisionMatrix.SetMatrix(collisionMatrix); }
    void SetMatrix(bool dD, bool dP, bool dE, bool dPPj, bool dEPj,
        bool pD, bool pP, bool pE, bool pPPj, bool pEPj,
        bool eD, bool eP, bool eE, bool ePPj, bool eEPj,
        bool ppjD, bool ppjP, bool ppjE, bool ppjPPj, bool ppjEPj,
        bool epjD, bool epjP, bool epjE, bool epjPPj, bool epjEPj);
    void RemoveAllColliders();
    void RemoveCollider(std::shared_ptr<Collider> collider);

private:
    void CollisionCheckAll();

public:
    void Update();
};

#endif
