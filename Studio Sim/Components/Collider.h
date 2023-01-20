#pragma once
#ifndef COLLIDER_H
#define COLLIDER_H

#include "Vector2f.h"
#include "Transform.h"

class BoxCollider;
class CircleCollider;

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

    int Clamp(int min, int max, int value);
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

    //Collision Checks
    virtual bool ToBox(BoxCollider* box) = 0;
    virtual bool ToCircle(CircleCollider* circle) = 0;
    virtual bool ToPoint(Vector2f point) = 0;
    bool CollisionCheck(Collider* collider);

    //Resolution
    virtual void Resolution(Collider* collider) = 0;
};

#endif