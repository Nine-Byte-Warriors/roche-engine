#include "stdafx.h"
#include "BoxCollider.h"

Vector2f BoxCollider::ClosestPoint(Vector2f targetPosition)
{
    Vector2f closestPoint;
    float halfHeight = (m_w / 2);
    float halfWidth = (m_h / 2);
    closestPoint.x = Clamp(m_tf->GetPosition().x - halfWidth, m_tf->GetPosition().x + halfWidth, targetPosition.x);
    closestPoint.y = Clamp(m_tf->GetPosition().y - halfHeight, m_tf->GetPosition().y + halfHeight, targetPosition.y);

    return closestPoint;
}

bool BoxCollider::ToBox(BoxCollider* box)
{
    float box1HalfWidth = (m_w / 2);
    float box1HalfHeight = (m_h / 2);

    float box2HalfWidth = (m_w / 2);
    float box2HalfHeight = (m_h / 2);

    Vector2f box1Min(m_tf->GetPosition().x - box1HalfWidth, m_tf->GetPosition().y - box1HalfHeight);
    Vector2f box1Max(m_tf->GetPosition().x + box1HalfWidth, m_tf->GetPosition().y + box1HalfHeight);

    Vector2f box2Min(m_tf->GetPosition().x - box1HalfWidth, m_tf->GetPosition().y - box2HalfHeight);
    Vector2f box2Max(m_tf->GetPosition().x + box1HalfWidth, m_tf->GetPosition().y + box2HalfHeight);

    //if (box1->GetTransform()->GetPosition().x < box2->GetTransform()->GetPosition().x + box2->GetWidth() / 2 &&
    //    box1->GetTransform()->GetPosition().x + box1->GetWidth() / 2 > box2->GetTransform()->GetPosition().x &&
    //    box1->GetTransform()->GetPosition().y < box2->GetTransform()->GetPosition().y + box2->GetHeight() / 2 &&
    //    box1->GetTransform()->GetPosition().y + box2->GetHeight() / 2 > box2->GetTransform()->GetPosition().y)
    //{
    //    return true;
    //}
    //else
    //{
    //    return false;
    //}

    if (box1Min.x < box2Max.x &&
        box1Max.x > box2Min.x &&
        box1Min.y < box2Max.y &&
        box1Max.y > box2Min.y)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool BoxCollider::ToCircle(CircleCollider* circle)
{
    Vector2f circlePos = circle->GetTransform()->GetPosition();
    Vector2f closestPoint = ClosestPoint(circlePos);

    int distance = (circlePos - closestPoint).Magnitude();

    if (distance < circle->GetRadius())
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool BoxCollider::ToPoint(Vector2f point)
{
    float boxHalfWidth = (m_w / 2);
    float boxHalfHeight = (m_h / 2);

    Vector2f boxMin(m_tf->GetPosition().x - boxHalfWidth, m_tf->GetPosition().y - boxHalfHeight);
    Vector2f boxMax(m_tf->GetPosition().x + boxHalfWidth, m_tf->GetPosition().y + boxHalfHeight);

    if (point.x < boxMax.x &&
        point.x > boxMin.x &&
        point.y < boxMax.y &&
        point.y >boxMin.y)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void BoxCollider::Resolution(Collider* collider)
{
    //Vector2f intersectDistance = closestPoint2 - closestPoint1;
//collider1->GetTransform()->SetPosition((collider1->GetTransform()->GetPosition() + intersectDistance));

    Vector2f closestPoint = ClosestPoint(collider1, collider2->GetTransform()->GetPosition());

    Vector2f lastValidPos = collider1->GetLastValidPosition();
    Vector2f newPos = collider1->GetTransform()->GetPosition();


    //Change the position on the x or y axis or both to move collider out of the other
    bool changeXValue = false;
    bool changeYValue = false;

    ColliderType collider2Type = collider2->GetColliderType();
    switch (collider2Type)
    {
    case ColliderType::Box:
    {
        BoxCollider* box = (BoxCollider*)collider2;
        changeXValue = !PointToBox(Vector2f(lastValidPos.x, closestPoint.y), box);
        changeYValue = !PointToBox(Vector2f(closestPoint.x, lastValidPos.y), box);
        break;
    }
    case ColliderType::Circle:
    {
        CircleCollider* circle = (CircleCollider*)collider2;
        changeXValue = !PointToCircle(Vector2f(lastValidPos.x, closestPoint.y), circle);
        changeYValue = !PointToCircle(Vector2f(closestPoint.x, lastValidPos.y), circle);
        break;
    }
    }

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

    collider1->GetTransform()->SetPosition(newPos);
}

bool BoxCollider::CollisionCheck(Collider* collider)
{
    return false;
}
