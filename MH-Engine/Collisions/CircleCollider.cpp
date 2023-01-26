#include "stdafx.h"
#include "CircleCollider.h"

Vector2f CircleCollider::ClosestPoint(Vector2f point) noexcept
{
    Vector2f direction = point - m_transform->GetPosition();
    Vector2f unit = direction.Divide(direction.Magnitude());

    Vector2f closestPoint = m_transform->GetPosition() + unit.Multiply(m_radius);

    return closestPoint;
}

Vector2f CircleCollider::ClosestSurfacePoint(Vector2f point) 
{
    Vector2f pos = m_transform->GetPosition();
    Vector2f direction = pos.DirectionTo(point);
    Vector2f result = direction.Multiply(m_radius);

    return result;
}

bool CircleCollider::ToBox(BoxCollider* box) noexcept
{
    Vector2f circlePos = m_transform->GetPosition();
    Vector2f closestPoint = box->ClosestPoint(circlePos);

    int distance = (circlePos - closestPoint).Magnitude();

    if (distance < m_radius)
        return true;

    return false;
}

bool CircleCollider::ToCircle(CircleCollider* circle) noexcept
{
    int radiusSum = m_radius + circle->GetRadius();
    int distance = Vector2f(m_transform->GetPosition().x - circle->GetTransform()->GetPosition().x,
                            m_transform->GetPosition().y - circle->GetTransform()->GetPosition().y).Magnitude();

    if (distance < radiusSum)
        return true;

    return false;
}

bool CircleCollider::ToPoint(Vector2f point) noexcept
{
    Vector2f circlePos = m_transform->GetPosition();

    int distance = (circlePos - point).Magnitude();

    if (distance < m_radius)
        return true;

    return false;
}

void CircleCollider::Resolution(Collider* collider) noexcept
{
    if (m_isTrigger)
        return;

    Vector2f newPos = m_transform->GetPosition();
    Vector2f lastValidPos = m_lastValidPosition;
    Vector2f closestPoint = ClosestPoint(collider->GetTransform()->GetPosition());

    bool changeXValue = false;
    bool changeYValue = false;

    switch (collider->GetColliderType())
    {
    case ColliderType::Box:
    {
        //Change the position on the x or y axis or both to move collider out of the other

        changeXValue = !collider->ToPoint(Vector2f(lastValidPos.x, closestPoint.y));
        changeYValue = !collider->ToPoint(Vector2f(closestPoint.x, lastValidPos.y));

        //change gameobjects position on either the x or y axis
        if (changeXValue)
        {
            newPos.x = lastValidPos.x;
        }
        //if changing the x works
        else if (changeYValue)
        {
            newPos.y = lastValidPos.y;
        }
        else if (changeXValue && changeYValue)
        {
            newPos = lastValidPos;
        }

        break;
    }
    case ColliderType::Circle:
    {

        //Change the position on the x or y axis or both to move collider out of the other
        changeXValue = !collider->ToPoint(Vector2f(lastValidPos.x, closestPoint.y));
        changeYValue = !collider->ToPoint(Vector2f(closestPoint.x, lastValidPos.y));

        //change gameobjects position on either the x or y axis
        if (changeXValue)
        {
            newPos.x = lastValidPos.x;
        }
        //if changing the x works
         else if (changeYValue)
        {
            newPos.y = lastValidPos.y;
        }
        else if (changeXValue && changeYValue)
        {
            newPos = lastValidPos;
        }

        break;
    }
    }

    m_transform->SetPosition(newPos);
}

bool CircleCollider::CollisionCheck(Collider* collider) noexcept
{
    switch (collider->GetColliderType())
    {
    case ColliderType::Box:
        return ToBox((BoxCollider*)collider);
    case ColliderType::Circle:
        return ToCircle((CircleCollider*)collider);
    }

    return false;
}