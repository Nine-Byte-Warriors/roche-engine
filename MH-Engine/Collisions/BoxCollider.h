#pragma once
#ifndef BOXCOLLIDER_H
#define BOXCOLLIDER_H

#include "Collision.h"

//AABB
class BoxCollider : public Collider
{
public:
    BoxCollider() { m_type = ColliderType::Box; };
    BoxCollider(std::shared_ptr<Transform> transform, int width, int height) : m_w(width), m_h(height) { m_tf = transform;/*m_tf->SetPosition(Vector2f(x, y));*/ m_type = ColliderType::Box; }

private:
    //position from bottom left
    float m_w = 0;
    float m_h = 0;
public:
    Vector2f ClosestPoint(Vector2f position) noexcept override;
    inline float GetWidth() const noexcept { return m_w; }
    inline float GetHeight() const noexcept { return m_h; }
    inline void SetWidth(float width) noexcept { m_w = width; }
    inline void SetHeight(float height) noexcept { m_h = height; }

    //Collision Checks
    bool ToBox(BoxCollider* box) noexcept override;
    bool ToCircle(CircleCollider* circle) noexcept override;
    bool ToPoint(Vector2f point) noexcept override;
    bool CollisionCheck(Collider* collider) noexcept override;

    Vector2f ClosestSurfacePoint(Vector2f point) noexcept;

    void Resolution(Collider* collider) noexcept override;
};

#endif