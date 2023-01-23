#pragma once
#ifndef CIRCLECOLLIDER_H
#define CIRCLECOLLIDER_H

//#include "Colliders.h"
#include "Collision.h"

class CircleCollider : public Collider
{
public:
    CircleCollider() { m_type = ColliderType::Circle; };
    CircleCollider(std::shared_ptr<Transform> transform, float radius) : m_radius(radius) { m_tf = transform; m_type = ColliderType::Circle; }

private:
    float m_radius = 0;
public:
    Vector2f ClosestPoint(Vector2f position) override;

    inline float GetRadius() { return m_radius; };
    inline void SetRadius(float radius) { m_radius = radius; };

    //Collision Checks
    bool ToBox(BoxCollider* box) override;
    bool ToCircle(CircleCollider* circle) override;
    bool ToPoint(Vector2f point) override;
    bool CollisionCheck(Collider* collider) override;


    //Resolution
    void Resolution(Collider* collider) override;
};
#endif