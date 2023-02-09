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
    Collider(
        const std::shared_ptr<Transform>& transform,
        const std::shared_ptr<Sprite>& sprite,
        bool trigger, int entityNum, std::string entityType );
    Collider(Collider& col);

private:
    bool m_isEnabledCopy;
protected:
    bool m_isEnabled = true;
    ColliderType m_type = ColliderType::None;
    bool m_isTrigger = false;
    bool m_isStatic = false;
    LayerMask m_collisionMask = LayerMask(true, true, true, true);
    LayerNo m_layer = LayerNo::Enemy;

    std::shared_ptr<Transform> m_transform;
    std::shared_ptr<Sprite> m_sprite;
    int m_entityNum;
    std::string m_entityType;

    Vector2f m_lastValidPosition = Vector2f(0, 0);

    const int m_maxCollisions = 50;
    int m_curCollisionCount = 0;
    std::vector<std::shared_ptr<Collider>> m_curCollisions;
    std::map<std::shared_ptr<Collider>, CollisionState> m_collisions;

    std::vector<std::function<void(Collider&)>> m_onEnterCallbacks;
    std::vector<std::function<void(Collider&)>> m_onLeaveCallbacks;

    std::vector<std::shared_ptr<Collider>> m_blackList;

public:
    //Helpers
    float Clamp(float min, float max, float value);
    template <typename T>
    void RemoveDuplicateElements(std::vector<T>& vec);

    //
    inline const ColliderType GetColliderType() { return m_type; }

    inline void SetIsTrigger(bool trigger) noexcept { m_isTrigger = trigger; }
    inline bool GetIsTrigger() noexcept { return m_isTrigger; };

    inline void SetIsEnabled(bool enabled) noexcept { m_isEnabled = enabled; }
    inline bool GetIsEnabled() noexcept { return m_isEnabled; }

    inline void SetIsStatic(bool isStatic) noexcept { m_isStatic = isStatic; }
    inline bool GetIsStatic() noexcept { return m_isStatic; };

    inline void SetLayer(LayerNo layer) { m_layer = layer; };
    inline LayerNo GetLayer() const noexcept { return m_layer; };

    inline void SetCollisionMask(LayerMask collisionMask) noexcept { m_collisionMask = collisionMask; }
    inline LayerMask GetCollisionMask() noexcept { return m_collisionMask; };

    inline std::shared_ptr<Transform> GetTransform() const noexcept { return m_transform; }
    inline void SetTransform(std::shared_ptr<Transform> tf) noexcept { m_transform = tf; }

    inline void SetEntityNum(int entityNum) { m_entityNum = entityNum; };
    inline int GetEntityNum() { return m_entityNum; };
    inline std::string EntityType() { return m_entityType; };

    Vector2f Offset();
    Vector2f GetCenterPosition();
    void SetTransformPosition(Vector2f position);

    inline Vector2f GetLastValidPosition() const noexcept { return m_lastValidPosition; }
    inline void UpdateLastValidPosition() noexcept { m_lastValidPosition = GetCenterPosition(); }//m_transform->GetPosition();} }//



    void LogCollision(std::shared_ptr<Collider>& col);


    virtual Vector2f ClosestPoint(Vector2f position) noexcept { return Vector2f(); }
    void CheckDisabled();

    //Collision Checks
    virtual bool ToBox(BoxCollider& box) noexcept { return false; }
    virtual bool ToCircle(CircleCollider& circle) noexcept { return false; }
    virtual bool ToPoint(Vector2f point) noexcept { return false; }
    virtual bool CollisionCheck(std::shared_ptr<Collider> collider) noexcept { return false; }

    //Collision Resolution
    bool ResolveCheck(std::shared_ptr<Collider> collider);
    virtual void Resolution(std::shared_ptr<Collider> collider) noexcept {};
    virtual void Resolve() {};

    //Events
    //example of use td::function<void(Collider&)> f = std::bind(&Player::foo, this, std::placeholders::_1);
    void OnEnter(Collider& col);
    void OnLeave(Collider& col);
    inline void AddOnEnterCallback(std::function<void(Collider&)> callback) { m_onEnterCallbacks.push_back(callback); };
    inline void AddOnExitCallback(std::function<void(Collider&)> callback) { m_onLeaveCallbacks.push_back(callback); };
    inline void ClearOnEnterCallbacks() { m_onEnterCallbacks.clear(); };
    inline void ClearOnExitCallbacks() { m_onLeaveCallbacks.clear(); };



    //Managing logged collisions against collider
    void ManageCollisions();
    void ProcessCollisions();

    virtual void Update();

    //Blacklist ignore specific colliders
    inline void AddToBlackList(std::shared_ptr<Collider> collider) { m_blackList.push_back(collider); };
    inline std::vector<std::shared_ptr<Collider>> GetBlackList() { return m_blackList; };
    inline void RemoveBlackListEntry(int element) { m_blackList.erase(m_blackList.begin() + element); }
    inline void ClearBlackList() { m_blackList.clear(); };
    bool CheckBlackList(std::shared_ptr<Collider> collider);

};

#endif