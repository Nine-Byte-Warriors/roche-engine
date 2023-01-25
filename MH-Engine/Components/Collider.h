#pragma once
#ifndef COLLIDER_H
#define COLLIDER_H

#include "Vector2f.h"
#include "Transform.h"
#include "LayerMask.h"
#include <functional>

const enum class ColliderType
{
    None,
    Box,
    Circle
};

enum class CollisionState
{
    Entering,
        Staying,
        Leaving
};

class BoxCollider;
class CircleCollider;
class Collider
{
public:
    Collider() {}
    Collider(bool trigger, std::shared_ptr<Transform>& transform) : m_isTrigger(trigger), m_tf(transform) {}
    Collider(Collider& col);

protected:
    ColliderType m_type = ColliderType::None;
    bool m_isTrigger = false;
    LayerMask m_collisionMask = LayerMask(true, true, true, true);
    LayerNo m_layer = LayerNo::Enemy;

    std::shared_ptr<Transform> m_tf;
    Vector2f m_lastValidPosition = Vector2f(0, 0);

    std::vector<std::shared_ptr<Collider>> m_curintersections;
    std::vector<std::pair<std::shared_ptr<Collider>, CollisionState>> m_intersections;

    std::vector<std::function<void(Collider&)>> m_onEnterCallbacks;
    std::vector<std::function<void(Collider&)>> m_onExitCallbacks;


public:
    inline const ColliderType GetColliderType() { return m_type; }

    inline bool GetIsTrigger() { return m_isTrigger; };
    inline void SetIsTrigger(bool trigger) { m_isTrigger = trigger; }

    void SetLayer(LayerNo layer) { m_layer = layer; };
    void SetCollisionMask(LayerMask collisionMask) { m_collisionMask = collisionMask; m_collisionMask = collisionMask; }
    LayerNo GetLayer() { return m_layer; };
    LayerMask GetLayerMask() { return m_collisionMask; };

    std::shared_ptr<Transform> GetTransform() { return m_tf; }

    inline Vector2f GetLastValidPosition() { return m_lastValidPosition; }
    inline void UpdateLastValidPosition() { m_lastValidPosition = m_tf->GetPosition(); }


    void LogCollision(std::shared_ptr<Collider>& col) { m_curintersections.push_back(col); }

    float Clamp(float min, float max, float value);

    virtual Vector2f ClosestPoint(Vector2f position) { return Vector2f(); }

    //Collision Checks
    virtual bool ToBox(BoxCollider* box) { return false; }
    virtual bool ToCircle(CircleCollider* circle) { return false; }
    virtual bool ToPoint(Vector2f point) { return false; }
    virtual bool CollisionCheck(Collider* collider) { return false; }

    //Resolution
    virtual void Resolution(Collider* collider) {}

    void AddOnEnterCallback(std::function<void(Collider&)> callback) { m_onEnterCallbacks.push_back(callback); };
    void AddOnExitCallback(std::function<void(Collider&)> callback) { m_onExitCallbacks.push_back(callback); };
    void ClearOnEnterCallbacks() { m_onEnterCallbacks.clear(); };
    void ClearOnExitCallbacks() { m_onExitCallbacks.clear(); };

    void ManageIntersections();
    template <typename T>
    std::vector <T> RemoveDuplicateEntries(std::vector<T> vec);
    void Process();
    void Update();

    //std::function<void(Collider&)> f = std::bind(&Player::foo, this, std::placeholders::_1);
    void OnEnter(Collider& col);
    void OnExit(Collider& col);
};

#endif