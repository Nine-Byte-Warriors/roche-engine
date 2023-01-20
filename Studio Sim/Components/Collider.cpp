#include "stdafx.h"
#include "Collider.h"

int Collider::Clamp(int min, int max, int value)
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

bool Collider::CollisionCheck(Collider* collider)
{
    switch (collider->GetColliderType())
    {
    case ColliderType::Box:
        ToBox((BoxCollider*)collider);
        break;
    case ColliderType::Circle:
        ToCircle((CircleCollider*)collider);
        break;
    }

    return false;
}

Vector2f Collider::ClosestPoint(Vector2f position)
{
    return Vector2f();
}
