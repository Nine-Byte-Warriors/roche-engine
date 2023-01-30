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
    float halfWidth = (m_width / 2);
    float halfHeight = (m_height / 2);
    float scalar = Vector2f(halfWidth, halfHeight).Length();


    Vector2f distance = point - position;
    Vector2f direction = distance.Normalised();
    Vector2f scaledVector = position + direction.Multiply(scalar);

    Vector2f surfacePoint;
    surfacePoint.x = Clamp((position.x - halfWidth), (position.x + halfWidth), scaledVector.x);
    surfacePoint.y = Clamp((position.y - halfHeight), (position.y + halfHeight), scaledVector.y);


    return surfacePoint;
}

bool BoxCollider::ToBox(BoxCollider& box) noexcept
{
    float box1HalfWidth = (m_width / 2);
    float box1HalfHeight = (m_height / 2);
    
    float box2HalfWidth = (m_width / 2);
    float box2HalfHeight = (m_height / 2);
    Vector2f box2Pos = box.GetTransform()->GetPosition();

    Vector2f box1Min(m_transform->GetPosition().x - box1HalfWidth, m_transform->GetPosition().y - box1HalfHeight);
    Vector2f box1Max(m_transform->GetPosition().x + box1HalfWidth, m_transform->GetPosition().y + box1HalfHeight);

    Vector2f box2Min(box2Pos.x - box1HalfWidth, box2Pos.y - box2HalfHeight);
    Vector2f box2Max(box2Pos.x + box1HalfWidth, box2Pos.y + box2HalfHeight);

    if (box1Min.x < box2Max.x &&
        box1Max.x > box2Min.x &&
        box1Min.y < box2Max.y &&
        box1Max.y > box2Min.y)
        return true;

    return false;
}

bool BoxCollider::ToCircle(CircleCollider& circle) noexcept
{
    Vector2f circlePos = circle.GetTransform()->GetPosition();
    Vector2f closestPoint = ClosestPoint(circlePos);

    float distance = (circlePos - closestPoint).Magnitude();

    if (distance < circle.GetRadius())
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


void BoxCollider::Resolution(std::shared_ptr<Collider> collider) noexcept
{
    if (m_isTrigger)
        return;

    Vector2f position = m_transform->GetPosition();
    Vector2f newPos = position;
    Vector2f closestPoint = ClosestSurfacePoint(collider->GetTransform()->GetPosition());

    bool changeXValue = false;
    bool changeYValue = false;


    switch (collider->GetColliderType())
    {
    case ColliderType::Box:
    {
        auto boxPtr = std::dynamic_pointer_cast<BoxCollider>(collider);
        BoxCollider box = *boxPtr;
        Vector2f shiftedY = Vector2f(m_lastValidPosition.x, closestPoint.y);
        Vector2f shiftedX = Vector2f(closestPoint.x, m_lastValidPosition.y);
        changeXValue = !box.ToPoint(shiftedY);
        changeYValue = !box.ToPoint(shiftedX);
        break;
    }
    case ColliderType::Circle:
    {
        auto circlePtr = std::dynamic_pointer_cast<CircleCollider>(collider);
        CircleCollider circle = *circlePtr;
        Vector2f shiftedY = Vector2f(m_lastValidPosition.x, closestPoint.y);
        Vector2f shiftedX = Vector2f(closestPoint.x, m_lastValidPosition.y);
        changeXValue = !circle.ToPoint(shiftedY);
        changeYValue = !circle.ToPoint(shiftedX);
        break;
    }
    }
    
    //change gameobjects position on either the x or y axis
    if (changeXValue)
    {
        newPos.x = m_lastValidPosition.x;
    }
    //if changing the x works
    else if (changeYValue)
    {
        newPos.y = m_lastValidPosition.y;
    }
    else if(changeXValue && changeYValue)
    {
        newPos = m_lastValidPosition;
    }

    m_transform->SetPosition(newPos);
}
bool BoxCollider::CollisionCheck(std::shared_ptr<Collider> collider) noexcept
{
    switch (collider->GetColliderType())
    {
    case ColliderType::Box:
        return ToBox(*std::dynamic_pointer_cast<BoxCollider>(collider));
    case ColliderType::Circle:
        return ToCircle(*std::dynamic_pointer_cast<CircleCollider>(collider));
    }

    return false;
}