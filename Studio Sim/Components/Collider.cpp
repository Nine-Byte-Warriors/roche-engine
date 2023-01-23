#include "stdafx.h"
#include "Collider.h"

float Collider::Clamp(float min, float max, float value)
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
