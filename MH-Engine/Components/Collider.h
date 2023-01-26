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
    std::map<std::shared_ptr<Collider>, CollisionState> m_intersections;

    std::vector<std::function<void(Collider&)>> m_onEnterCallbacks;
    std::vector<std::function<void(Collider&)>> m_onExitCallbacks;

public:
    inline const ColliderType GetColliderType() { return m_type; }

    inline bool GetIsTrigger() noexcept { return m_isTrigger; };
    inline void SetIsTrigger(bool trigger) noexcept { m_isTrigger = trigger; }

    inline void SetLayer(LayerNo layer) { m_layer = layer; };
    inline void SetCollisionMask(LayerMask collisionMask) noexcept { m_collisionMask = collisionMask; m_collisionMask = collisionMask; }
    inline LayerNo GetLayer() const noexcept { return m_layer; };
    inline LayerMask GetLayerMask() noexcept { return m_collisionMask; };
    inline std::shared_ptr<Transform> GetTransform() const noexcept { return m_tf; }

    inline Vector2f GetLastValidPosition() const noexcept { return m_lastValidPosition; }
    inline void UpdateLastValidPosition() noexcept { m_lastValidPosition = m_tf->GetPosition(); }
    inline void LogCollision(std::shared_ptr<Collider>& col) { m_curintersections.push_back(col); }

    float Clamp(float min, float max, float value);
    virtual Vector2f ClosestPoint(Vector2f position) noexcept { return Vector2f(); }

    //Collision Checks
    virtual bool ToBox(BoxCollider* box) noexcept { return false; }
    virtual bool ToCircle(CircleCollider* circle) noexcept { return false; }
    virtual bool ToPoint(Vector2f point) noexcept { return false; }
    virtual bool CollisionCheck(Collider* collider) noexcept { return false; }

    //Resolution
    virtual void Resolution(Collider* collider) noexcept {}

    inline void AddOnEnterCallback(std::function<void(Collider&)> callback) { m_onEnterCallbacks.push_back(callback); };
    inline void AddOnExitCallback(std::function<void(Collider&)> callback) { m_onExitCallbacks.push_back(callback); };
    inline void ClearOnEnterCallbacks() { m_onEnterCallbacks.clear(); };
    inline void ClearOnExitCallbacks() { m_onExitCallbacks.clear(); };

    void ManageIntersections();
    template <typename T>
    void RemoveDuplicateEntries(std::vector<T>& vec);
    void Process();
    void Update();

    //std::function<void(Collider&)> f = std::bind(&Player::foo, this, std::placeholders::_1);
    void OnEnter(Collider& col);
    void OnExit(Collider& col);
};

#endif