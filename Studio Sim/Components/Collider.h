#pragma once
#ifndef COLLIDER_H
#define COLLIDER_H

#include "Vector2f.h"
#include "Transform.h"

const enum class ColliderType
{
    None,
    Box,
    Circle
};

class Collider
{
public:
    Collider(){}
    Collider(bool trigger, std::shared_ptr<Transform>& transform) : m_isTrigger(trigger), m_tf(transform) {};
    virtual Vector2f ClosestPoint(Vector2f position) = 0;

protected:
    ColliderType m_type = ColliderType::None;
public:
    inline const ColliderType GetColliderType() { return m_type; }

protected:
    bool m_isTrigger = false;
public:
    inline bool GetIsTrigger() { return m_isTrigger; };
    inline void SetIsTrigger(bool trigger) { m_isTrigger = trigger; }

private:
    Vector2f m_lastValidPosition = Vector2f(0,0);
public:
    inline Vector2f GetLastValidPosition() { return m_lastValidPosition; }
    inline void UpdateLastValidPosition() { m_lastValidPosition = m_tf->GetPosition(); };
    
protected:
    std::shared_ptr<Transform> m_tf;
public:
    std::shared_ptr<Transform> GetTransform() { return m_tf; }
};

//AABB
class BoxCollider : public Collider
{
public:
    BoxCollider()  { m_type = ColliderType::Box; };
    BoxCollider(std::shared_ptr<Transform> transform, int x, int y, int width, int height) : m_w(width), m_h(height) { m_tf = transform;/*m_tf->SetPosition(Vector2f(x, y));*/ m_type = ColliderType::Box; }

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
};

class CircleCollider : public Collider
{
public:
    CircleCollider() { m_type = ColliderType::Circle; };
    CircleCollider(std::shared_ptr<Transform> transform, int x, int y, float radius) : m_radius(radius) { m_tf = transform; m_type = ColliderType::Circle; }

private:
    float m_radius = 0;
public:
    Vector2f ClosestPoint(Vector2f position) override;

    inline float GetRadius() { return m_radius; };
    inline void SetRadius(float radius) { m_radius = radius; };
};

#endif