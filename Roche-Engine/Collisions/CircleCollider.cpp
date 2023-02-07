#include "stdafx.h"
#include "CircleCollider.h"

CircleCollider::CircleCollider(
    const std::shared_ptr<Transform>& transform,
    const std::shared_ptr<Sprite>& sprite,
    bool trigger, int entityNum, std::string entityType, float radius )
{
    m_transform = transform;
    m_sprite = sprite;
    m_isTrigger = trigger;

    m_entityNum = entityNum;
    m_entityType = entityType;

    m_radius = radius;
    m_type = ColliderType::Circle;
}

Vector2f CircleCollider::ClosestPoint(Vector2f point) noexcept
{
    Vector2f closestPoint = point;
    Vector2f direction = point - GetCenterPosition();
    float distance = (point - GetCenterPosition()).Magnitude();

    if (distance > m_radius)
    {
        Vector2f unit = direction.Normalised();
        closestPoint = GetCenterPosition() + unit.Multiply(m_radius);
    }

    //------------------------------- WAY IT WAS BEFORE ----------------------
    //Vector2f direction = point - m_transform->GetPosition();
    //Vector2f unit = direction.Divide(direction.Magnitude());

    //Vector2f closestPoint = m_transform->GetPosition() + unit.Multiply(m_radius);

    return closestPoint;
}

Vector2f CircleCollider::ClosestSurfacePoint(Vector2f point)
{
    Vector2f direction = point - GetCenterPosition();
    Vector2f unit = direction.Divide(direction.Magnitude());

    Vector2f closestSurfacePoint = GetCenterPosition() + unit.Multiply(m_radius);

    return closestSurfacePoint;
}

bool CircleCollider::ToBox(BoxCollider& box) noexcept
{
    Vector2f circlePos = GetCenterPosition();
    Vector2f closestPoint = box.ClosestPoint(circlePos);

    float distance = (circlePos - closestPoint).Magnitude();

    if (distance < m_radius)
        return true;

    return false;
}

bool CircleCollider::ToCircle(CircleCollider& circle) noexcept
{
    float radiusSum = m_radius + circle.GetRadius();

    Vector2f currTransform = GetCenterPosition();
    Vector2f targetTransform = circle.GetCenterPosition();
    float distance = (targetTransform - currTransform).Magnitude();

    //------------------------------- WAY IT WAS BEFORE ----------------------
    //float distance = Vector2f(
    //    m_transform->GetPosition().x - circle.GetTransform()->GetPosition().x,
    //    m_transform->GetPosition().y - circle.GetTransform()->GetPosition().y).Magnitude();

    //
    if (distance < radiusSum)
    {
        return true;
    }

    return false;
}

bool CircleCollider::ToPoint(Vector2f point) noexcept
{
    Vector2f circlePos = GetCenterPosition();

    float distance = (circlePos - point).Magnitude();

    if (distance < m_radius)
        return true;

    return false;
}

void CircleCollider::Resolution(std::shared_ptr<Collider> collider) noexcept
{
    if (ResolveCheck(collider) == false)
    {
        return;
    }

    //Vector2f position = m_transform->GetPosition();
    Vector2f newPos = GetCenterPosition();

    switch (collider->GetColliderType())
    {
    case ColliderType::Box:
        {
            //Vector2f closestPoint = ClosestPoint(GetCenterPosition());
            //bool changeXValue = false;
            //bool changeYValue = false;

            auto boxPtr = std::dynamic_pointer_cast<BoxCollider>(collider);
            BoxCollider box = *boxPtr;

            box.SetHeight(box.GetHeight() + (m_radius * 2) );
            box.SetWidth(box.GetWidth() + (m_radius * 2) );

            newPos = box.ClosestSurfacePoint(GetCenterPosition());

            //Vector2f shiftedY = Vector2f(m_lastValidPosition.x, closestPoint.y);
            //Vector2f shiftedX = Vector2f(closestPoint.x, m_lastValidPosition.y);
            //changeXValue = !box.ToPoint(shiftedY);
            //changeYValue = !box.ToPoint(shiftedX);

            ////change gameobjects position on either the x or y axis
            //if (changeXValue)
            //{
            //    newPos.x = m_lastValidPosition.x;
            //}
            ////if changing the x works
            //if (changeYValue)
            //{
            //    newPos.y = m_lastValidPosition.y;
            //}

            break;
        }
    case ColliderType::Circle:
        {
            auto circlePtr = std::dynamic_pointer_cast<CircleCollider>(collider);
            CircleCollider circle = *circlePtr;
            circle.SetRadius(circle.GetRadius() + m_radius);
            newPos = circle.ClosestSurfacePoint(GetCenterPosition());

            //------------------------------- WAY IT WAS BEFORE ----------------------
            //auto circlePtr = std::dynamic_pointer_cast<CircleCollider>(collider);
            //CircleCollider circle = *circlePtr;
            //circle.SetRadius(circle.GetRadius() + m_radius);
            //newPos = circle.ClosestPoint(m_transform->GetPosition());
            break;
        }
    }

    SetTransformPosition(newPos);
    //m_transform->SetPosition(newPos);
}

bool CircleCollider::CollisionCheck(std::shared_ptr<Collider> collider) noexcept
{
    if (CheckBlackList(collider) == true)
    {
        return false;
    }

    switch (collider->GetColliderType())
    {
    case ColliderType::Box:
        return ToBox(*std::dynamic_pointer_cast<BoxCollider>(collider));
    case ColliderType::Circle:
        return ToCircle(*std::dynamic_pointer_cast<CircleCollider>(collider));
    }

    return false;
}