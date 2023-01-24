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

Vector2f BoxCollider::ClosestSurfacePoint(Vector2f point)
{
    Vector2f position = m_tf->GetPosition();

    Vector2f direction = (point - position).Normalised();
    Vector2f projectedVector;

    if (m_w > m_h)
    {
        projectedVector = position + direction.Multiply(m_w);
    }
    else
    {
        projectedVector = position + direction.Multiply(m_h);
    }

    float surfacePointx = Clamp((position.x - m_w), (position.x + m_w), projectedVector.x);
    float surfacePointy = Clamp((position.y - m_h), (position.x + m_h), projectedVector.y);

    Vector2f surfacePoint = Vector2f(surfacePointx, surfacePointy);
    return surfacePoint;
}
void BoxCollider::Resolution(Collider* collider)
{
    if (m_isTrigger == true)
    {
        return;
    }

    Vector2f newPos = m_tf->GetPosition();
    
    switch (collider->GetColliderType())
    {
    case ColliderType::Box:
    {
        BoxCollider* box = (BoxCollider*)collider;
        BoxCollider temp = *box;
        float width = temp.GetWidth();
        float height = temp.GetHeight();

        temp.SetHeight(height + (m_h/2));
        temp.SetWidth(width + (m_w / 2));
        Vector2f position = m_tf->GetPosition();

        newPos = ClosestSurfacePoint(m_tf->GetPosition());//

        //changeXValue = !ToPoint(Vector2f(lastValidPos.x, m_tf->GetPosition().x);
        //changeYValue = !ToPoint(Vector2f(closestPoint.x, lastValidPos.y));
        break;
    }
    case ColliderType::Circle:
    {

        Vector2f closestPoint = ClosestPoint(collider->GetTransform()->GetPosition());

        Vector2f lastValidPos = m_lastValidPosition;
        //Change the position on the x or y axis or both to move collider out of the other
        bool changeXValue = false;
        bool changeYValue = false;

        changeXValue = !ToPoint(Vector2f(lastValidPos.x, closestPoint.y));
        changeYValue = !ToPoint(Vector2f(closestPoint.x, lastValidPos.y));

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
    
    m_tf->SetPosition(newPos);


}
bool BoxCollider::CollisionCheck(Collider* collider)
{
    switch (collider->GetColliderType())
    {
    case ColliderType::Box:
        return ToBox((BoxCollider*)collider);
        break;
    case ColliderType::Circle:
        return ToCircle((CircleCollider*)collider);
        break;
    }

    return false;
}