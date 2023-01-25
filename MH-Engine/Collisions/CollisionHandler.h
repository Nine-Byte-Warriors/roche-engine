#pragma once
#ifndef COLLISIONHANDLER_H
#define COLLISIONHANDLER_H

#include "Collider.h"

class CollisionHandler
{
public:
    CollisionHandler() = default;

private:
    std::vector<Collider*> m_colliders;
public:
    void AddCollider(std::shared_ptr<Collider> collider);
    void AddCollider(std::vector<Collider>& colliders);
    void RemoveAllColliders();

private:
    bool BoxToBox(BoxCollider* box1, BoxCollider* box2);
    bool CircleToBox(CircleCollider* circle, BoxCollider* box);
    bool CircleToCircle(CircleCollider* circle1, CircleCollider* circle2);
    bool CollisionChecker(Collider* collider1, Collider* collider2);

private:
    void CheckAll();

    void Resolution(Collider* collider1, Collider* collider2);

public:
    void Update();
};

#endif