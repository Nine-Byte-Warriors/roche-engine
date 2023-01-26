#pragma once
#ifndef BOXCOLLIDER_H
#define BOXCOLLIDER_H

#include "Collision.h"

//AABB
class BoxCollider : public Collider
{
public:
    BoxCollider() { m_type = ColliderType::Box; };
    BoxCollider(std::shared_ptr<Transform> transform, int width, int height) : m_width(width), m_height(height) { m_transform = transform;/*m_tf->SetPosition(Vector2f(x, y));*/ m_type = ColliderType::Box; }

private:
    //position is from the center
    float m_width = 0;
    float m_height = 0;
public:
    inline float GetWidth() const noexcept { return m_width; }
    inline float GetHeight() const noexcept { return m_height; }
    inline void SetWidth(float width) noexcept { m_width = width; }
    inline void SetHeight(float height) noexcept { m_height = height; }

    Vector2f ClosestPoint(Vector2f position) noexcept override;
    Vector2f ClosestSurfacePoint(Vector2f point) noexcept;


    //Collision Checks
    bool ToBox(BoxCollider* box) noexcept override;
    bool ToCircle(CircleCollider* circle) noexcept override;
    bool ToPoint(Vector2f point) noexcept override;
    bool CollisionCheck(Collider* collider) noexcept override;


    void Resolution(Collider* collider) noexcept override;
};

#endif