#pragma once
#ifndef COLLIDER_H
#define COLLIDER_H

#include "Vector2f.h"
#include "Transform.h"
#include "LayerMask.h"

const enum class ColliderType
{
    None,
    Box,
    Circle
};

class BoxCollider;
class CircleCollider;
class Collider
{
public:
    Collider(){}
    Collider(bool trigger, std::shared_ptr<Transform>& transform) : m_isTrigger(trigger), m_tf(transform) {};

protected:
    ColliderType m_type = ColliderType::None;
public:
    inline const ColliderType GetColliderType() { return m_type; }

protected:
    bool m_isTrigger = false;
    //bool m_isStatic = false;
public:
    inline bool GetIsTrigger() { return m_isTrigger; };
    inline void SetIsTrigger(bool trigger) { m_isTrigger = trigger; }

protected:
    Vector2f m_lastValidPosition = Vector2f(0,0);
public:
    inline Vector2f GetLastValidPosition() { return m_lastValidPosition; }
    inline void UpdateLastValidPosition() { m_lastValidPosition = m_tf->GetPosition(); };
    
protected:
    std::shared_ptr<Transform> m_tf;
public:
    std::shared_ptr<Transform> GetTransform() { return m_tf; }

protected:
    LayerMask m_collisionMask = LayerMask(true,true,true,true);
    LayerNo m_layer =LayerNo::Enemy;

public:
    void SetLayer(LayerNo layer) { m_layer = layer; };
    void SetCollisionMask(LayerMask collisionMask) { m_collisionMask = collisionMask; m_collisionMask = collisionMask; };
    LayerNo GetLayer() { return m_layer; };
    LayerMask GetLayerMask() { return m_collisionMask; };

    float Clamp(float min, float max, float value);

    virtual Vector2f ClosestPoint(Vector2f position) = 0;

    //Collision Checks
    virtual bool ToBox(BoxCollider* box) = 0;
    virtual bool ToCircle(CircleCollider* circle) = 0;
    virtual bool ToPoint(Vector2f point) = 0;
    virtual bool CollisionCheck(Collider* collider) = 0;

    //Resolution
    virtual void Resolution(Collider* collider) = 0;
};

#endif