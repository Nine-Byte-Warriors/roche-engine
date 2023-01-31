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
    void SetMatrix(bool dD, bool dP, bool dE, bool dPj,
        bool pD, bool pP, bool pE, bool pPj,
        bool eD, bool eP, bool eE, bool ePj,
        bool pjD, bool pjP, bool pjE, bool pjPj);
    void RemoveAllColliders();

private:
    void CollisionCheckAll();

public:
    void Update();
};

#endif
