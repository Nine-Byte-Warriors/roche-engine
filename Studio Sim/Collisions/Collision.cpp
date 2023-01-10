#include "stdafx.h"
#include "Collision.h"

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

const bool Collision::BoxToBox(BoxCollider box1, BoxCollider box2)
{
    if (box1.m_position.x < box2.m_position.x + box2.m_w &&
        box1.m_position.x + box1.m_w > box2.m_position.x &&
        box1.m_position.y < box2.m_position.y + box2.m_h &&
        box1.m_position.y + box2.m_h > box2.m_position.y)
    {
        return true;
    }
    else
    {
        return false;
    }
}
const bool Collision::CircleToBox(CircleCollider circle, BoxCollider box)
{
    Vector2f closestPoint;
    closestPoint.x = Clamp(box.m_position.x, box.m_position.x + box.m_w, closestPoint.x);
    closestPoint.y = Clamp(box.m_position.y, box.m_position.y + box.m_h, closestPoint.y);
    Vector2f circlePos(circle.m_position.x, circle.m_position.y);

    int distance = (circlePos - closestPoint).Magnitude();

    if (distance < circle.m_radius)
    {
        return true;
    }
    else
    {
        return false;
    }
}
const bool Collision::CircleToCircle(CircleCollider circle1, CircleCollider circle2)
{
    int radiusSum = circle1.m_radius + circle2.m_radius;
    int distance = Vector2f(circle1.m_position.x - circle2.m_position.x, circle1.m_position.y - circle2.m_position.y).Magnitude();

    if (distance < radiusSum)
    {
        return true;
    }
    else
    {
        return false;
    }
}

