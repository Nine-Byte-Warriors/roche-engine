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
    closestPoint.x = Clamp(m_tf->GetPosition().x, m_tf->GetPosition().x + m_w, targetPosition.x);
    closestPoint.y = Clamp(m_tf->GetPosition().y, m_tf->GetPosition().y + m_h, targetPosition.y);

    return closestPoint;
}

Vector2f CircleCollider::ClosestPoint(Vector2f position)
{
    Vector2f direction = position - m_tf->GetPosition();
    Vector2f unit = direction.Divide(direction.Magnitude());
    
    Vector2f closestPoint = m_tf->GetPosition() + unit.Multiply(m_radius);

    return closestPoint;
}
