#pragma once
#ifndef CIRCLECOLLIDER_H
#define CIRCLECOLLIDER_H

//#include "Colliders.h"
#include "Collision.h"

class CircleCollider : public Collider
{
public:
    CircleCollider() { m_type = ColliderType::Circle; };
    CircleCollider(std::shared_ptr<Transform> transform, float radius) : m_radius(radius/2) { m_transform = transform; m_type = ColliderType::Circle; }

private:
    float m_radius = 0;
public:
    inline float GetRadius() const noexcept { return m_radius; };
    inline void SetRadius(float radius) noexcept { m_radius = radius; };

    Vector2f ClosestPoint(Vector2f point) noexcept override;
    Vector2f ClosestSurfacePoint(Vector2f point) ;

    //Collision Checks
    bool ToBox(BoxCollider& box) noexcept override;
    bool ToCircle(CircleCollider& circle) noexcept override;
    bool ToPoint(Vector2f point) noexcept override;
    bool CollisionCheck(std::shared_ptr<Collider> collider) noexcept override;

    //Resolution
    void Resolution(std::shared_ptr<Collider> collider) noexcept override;
};
#endif