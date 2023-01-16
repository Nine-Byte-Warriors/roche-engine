#pragma once
#include "Collider.h"
#include <vector>
#include <memory>

class CollisionHandler
{
public:
    CollisionHandler() = default;

private:
    std::vector<Collider*> m_colliders;
public:
    void AddCollider(Collider& collider);
    void AddCollider(std::vector<Collider>& colliders);

private:
    bool BoxToBox(BoxCollider* box1, BoxCollider* box2);
    bool CircleToBox(CircleCollider* circle, BoxCollider* box);
    bool CircleToCircle(CircleCollider* circle1, CircleCollider* circle2);
    bool CollisionChecker(Collider* collider1, Collider* collider2);

private:
    void CheckAll();

    void Resolution(Collider& collider1, Collider& collider2);

public:
    void Update();
};

