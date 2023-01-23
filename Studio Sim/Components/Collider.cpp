#include "stdafx.h"
#include "Collider.h"

int Clamp(int min, int max, int value)
{
    if (value > max)
    {
        return max;
    }
    else if (value < min)
    {
        return min;
    }
    else
    {
        return value;
    }
}

Vector2f Collider::ClosestPoint(Vector2f position)
{
    return Vector2f();
}

Vector2f BoxCollider::ClosestPoint(Vector2f targetPosition)
{
    Vector2f closestPoint;
    float halfHeight = (m_w / 2);
    float halfWidth = (m_h / 2);
    closestPoint.x = Clamp(m_tf->GetPosition().x - halfWidth, m_tf->GetPosition().x + halfWidth, targetPosition.x);
    closestPoint.y = Clamp(m_tf->GetPosition().y - halfHeight, m_tf->GetPosition().y + halfHeight, targetPosition.y);

    return closestPoint;
}

Vector2f CircleCollider::ClosestPoint(Vector2f position)
{
    Vector2f direction = position - m_tf->GetPosition();
    Vector2f unit = direction.Divide(direction.Magnitude());
    
    Vector2f closestPoint = m_tf->GetPosition() + unit.Multiply(m_radius);

    return closestPoint;
}
