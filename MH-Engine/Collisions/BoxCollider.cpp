#include "stdafx.h"
#include "BoxCollider.h"

Vector2f BoxCollider::ClosestPoint(Vector2f targetPosition) noexcept
{
    Vector2f closestPoint;
    float halfHeight = (m_width / 2);
    float halfWidth = (m_height / 2);
    closestPoint.x = Clamp(m_transform->GetPosition().x - halfWidth, m_transform->GetPosition().x + halfWidth, targetPosition.x);
    closestPoint.y = Clamp(m_transform->GetPosition().y - halfHeight, m_transform->GetPosition().y + halfHeight, targetPosition.y);

    return closestPoint;
}
Vector2f BoxCollider::ClosestSurfacePoint(Vector2f point) noexcept
{
    Vector2f position = m_transform->GetPosition();

    Vector2f direction = (point - position).Normalised();
    Vector2f projectedVector;

    if (m_width > m_height)
    {
        projectedVector = position + direction.Multiply(m_width);
    }
    else
    {
        projectedVector = position + direction.Multiply(m_height);
    }

    float surfacePointx = Clamp((position.x - m_width), (position.x + m_width), projectedVector.x);
    float surfacePointy = Clamp((position.y - m_height), (position.x + m_height), projectedVector.y);

    Vector2f surfacePoint = Vector2f(surfacePointx, surfacePointy);
    return surfacePoint;
}

bool BoxCollider::ToBox(BoxCollider* box) noexcept
{
    float box1HalfWidth = (m_width / 2);
    float box1HalfHeight = (m_height / 2);

    float box2HalfWidth = (m_width / 2);
    float box2HalfHeight = (m_height / 2);
    Transform& box2Tf = *box->GetTransform();

    Vector2f box1Min(m_transform->GetPosition().x - box1HalfWidth, m_transform->GetPosition().y - box1HalfHeight);
    Vector2f box1Max(m_transform->GetPosition().x + box1HalfWidth, m_transform->GetPosition().y + box1HalfHeight);

    Vector2f box2Min(box2Tf.GetPosition().x - box1HalfWidth, box2Tf.GetPosition().y - box2HalfHeight);
    Vector2f box2Max(box2Tf.GetPosition().x + box1HalfWidth, box2Tf.GetPosition().y + box2HalfHeight);

    if (box1Min.x < box2Max.x &&
        box1Max.x > box2Min.x &&
        box1Min.y < box2Max.y &&
        box1Max.y > box2Min.y)
        return true;

    return false;
}

bool BoxCollider::ToCircle(CircleCollider* circle) noexcept
{
    Vector2f circlePos = circle->GetTransform()->GetPosition();
    Vector2f closestPoint = ClosestPoint(circlePos);

    int distance = (circlePos - closestPoint).Magnitude();

    if (distance < circle->GetRadius())
        return true;

    return false;
}

bool BoxCollider::ToPoint(Vector2f point) noexcept
{
    float boxHalfWidth = (m_width / 2);
    float boxHalfHeight = (m_height / 2);

    Vector2f boxMin(m_transform->GetPosition().x - boxHalfWidth, m_transform->GetPosition().y - boxHalfHeight);
    Vector2f boxMax(m_transform->GetPosition().x + boxHalfWidth, m_transform->GetPosition().y + boxHalfHeight);

    if (point.x < boxMax.x &&
        point.x > boxMin.x &&
        point.y < boxMax.y &&
        point.y >boxMin.y)
        return true;

    return false;
}


void BoxCollider::Resolution(Collider*  collider) noexcept
{
    if (m_isTrigger)
        return;

    Vector2f newPos = m_transform->GetPosition();

    bool changeXValue = false;
    bool changeYValue = false;
    Vector2f lastValidPos = m_lastValidPosition;

    Vector2f closestPoint = ClosestPoint(collider->GetTransform()->GetPosition());
    switch (collider->GetColliderType())
    {
    case ColliderType::Box:
    {
        BoxCollider* box = (BoxCollider*)collider;
        BoxCollider temp = *box;
        float width = temp.GetWidth();
        float height = temp.GetHeight();

        temp.SetHeight(height + (m_height/2));
        temp.SetWidth(width + (m_width / 2));
        Vector2f position = m_transform->GetPosition();

        newPos = ClosestSurfacePoint(m_transform->GetPosition());//

        changeXValue = !ToPoint(Vector2f(lastValidPos.x, m_transform->GetPosition().x));
        changeYValue = !ToPoint(Vector2f(closestPoint.x, lastValidPos.y));
        break;
    }
    case ColliderType::Circle:
    {
        //Change the position on the x or y axis or both to move collider out of the other

        changeXValue = !ToPoint(Vector2f(lastValidPos.x, closestPoint.y));
        changeYValue = !ToPoint(Vector2f(closestPoint.x, lastValidPos.y));

        break;
    }
    }

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
    m_transform->SetPosition(newPos);


}
bool BoxCollider::CollisionCheck(Collider* collider) noexcept
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