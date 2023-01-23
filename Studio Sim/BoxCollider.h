#pragma once
#ifndef BOXCOLLIDER_H
#define BOXCOLLIDER_H

#include "Colliders.h"

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
    Vector2f ClosestPoint(Vector2f position) override;
    inline float GetWidth() { return m_w; }
    inline float GetHeight() { return m_h; }
    inline void SetWidth(float width) { m_w = width; }
    inline void SetHeight(float height) { m_h = height; }

    //Collision Checks
    bool ToBox(BoxCollider* box) override;
    bool ToCircle(CircleCollider* circle) override;
    bool ToPoint(Vector2f point) override;
    bool CollisionCheck(Collider* collider) override;

    Vector2f ClosestSurfacePoint(Vector2f point);

    void Resolution(Collider* collider) override;
};

#endif