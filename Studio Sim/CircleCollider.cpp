#include "stdafx.h"
#include "CircleCollider.h"


Vector2f CircleCollider::ClosestPoint(Vector2f position)
{
    Vector2f direction = position - m_tf->GetPosition();
    Vector2f unit = direction.Divide(direction.Magnitude());

    Vector2f closestPoint = m_tf->GetPosition() + unit.Multiply(m_radius);

    return closestPoint;
}

bool CircleCollider::ToBox(BoxCollider* box)
{
    Vector2f circlePos = m_tf->GetPosition();
    Vector2f closestPoint = box->ClosestPoint(circlePos);

    int distance = (circlePos - closestPoint).Magnitude();

    if (distance < m_radius)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool CircleCollider::ToCircle(CircleCollider* circle)
{
    int radiusSum = m_radius + circle->GetRadius();
    int distance = Vector2f(m_tf->GetPosition().x - circle->GetTransform()->GetPosition().x,
                            m_tf->GetPosition().y - circle->GetTransform()->GetPosition().y).Magnitude();

    if (distance < radiusSum)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool CircleCollider::ToPoint(Vector2f point)
{
    Vector2f circlePos = m_tf->GetPosition();

    int distance = (circlePos - point).Magnitude();

    if (distance < m_radius)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void CircleCollider::Resolution(Collider* collider)
{
    //Vector2f intersectDistance = closestPoint2 - closestPoint1;
//collider1->GetTransform()->SetPosition((collider1->GetTransform()->GetPosition() + intersectDistance));

    Vector2f closestPoint = ClosestPoint(collider->GetTransform()->GetPosition());

    Vector2f lastValidPos = collider->GetLastValidPosition();
    Vector2f newPos = collider->GetTransform()->GetPosition();


    //Change the position on the x or y axis or both to move collider out of the other
    bool changeXValue = false;
    bool changeYValue = false;



    //change gameobjects position on either the x or y axis
    if (changeXValue)
    {
        newPos.x = lastValidPos.x;
    }
    //if changing the x works
    if (changeYValue)
    {
        newPos.y = lastValidPos.y;
    }

    collider->GetTransform()->SetPosition(newPos);
}

