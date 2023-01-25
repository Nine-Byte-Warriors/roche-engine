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

private:
    bool BoxToBox(BoxCollider* box1, BoxCollider* box2);
    bool CircleToBox(CircleCollider* circle, BoxCollider* box);
    bool CircleToBox(BoxCollider* box, CircleCollider* circle);
    bool CircleToCircle(CircleCollider* circle1, CircleCollider* circle2);
    bool PointToBox(Vector2f point, BoxCollider* box);
    bool PointToCircle(Vector2f point, CircleCollider* circle);
    //Check for collisions between any type of collider
    bool CollisionCheck(Collider* collider1, Collider* collider2);

private:
    void CollisionCheckAll();

    void Resolution(Collider* collider1, Collider* collider2);

    //Closest Point on a collider to another collider
    Vector2f ClosestPoint(Collider* collider1, Vector2f collider2);

public:
    void Update();
};

#endif