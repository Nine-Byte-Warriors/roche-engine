#include "stdafx.h"
#include "CircleCollider.h"

Vector2f CircleCollider::ClosestPoint(Vector2f point) noexcept
{
    Vector2f direction = point - m_transform->GetPosition();
    Vector2f unit = direction.Divide(direction.Magnitude());

    Vector2f closestPoint = m_transform->GetPosition() + unit.Multiply(m_radius);

    return closestPoint;
}

Vector2f CircleCollider::ClosestSurfacePoint(Vector2f point) 
{
    Vector2f pos = m_transform->GetPosition();
    Vector2f direction = pos.DirectionTo(point);
    Vector2f result = direction.Multiply(m_radius);

    return result;
}

bool CircleCollider::ToBox(BoxCollider& box) noexcept
{
    Vector2f circlePos = m_transform->GetPosition();
    Vector2f closestPoint = box.ClosestPoint(circlePos);

    float distance = (circlePos - closestPoint).Magnitude();

    if (distance < m_radius)
        return true;

    return false;
}

bool CircleCollider::ToCircle(CircleCollider& circle) noexcept
{
    float radiusSum = m_radius + circle.GetRadius();
    //float distance = Vector2f(
    //    ( m_transform->GetPosition().x + m_transform->GetSprite()->GetWidth() / 2.0f ) -
    //        ( circle.GetTransform()->GetPosition().x + circle.GetTransform()->GetSprite()->GetWidth() / 2.0f ),
    //    ( m_transform->GetPosition().y + m_transform->GetSprite()->GetHeight() / 2.0f ) -
    //        ( circle.GetTransform()->GetPosition().y + circle.GetTransform()->GetSprite()->GetHeight() / 2.0f ) ).Magnitude();

    //Vector2f position = m_transform->GetPosition();
    //Vector2f circlePosition = circle.GetTransform()->GetPosition();
    //Vector2f sp = Vector2f(position.x + m_transform->GetSprite()->GetWidth() / 2.0f, position.y + m_transform->GetSprite()->GetHeight() / 2.0f);
    //Vector2f cp = Vector2f(circlePosition.x + m_transform->GetSprite()->GetWidth() / 2.0f, circlePosition.y + m_transform->GetSprite()->GetHeight() / 2.0f);
    
    //float distance = sp.Distance(cp);

    //Vector2f currTransform = Vector2f( m_transform->GetPosition() /*- Vector2f( m_transform->GetSprite()->GetWidth(), m_transform->GetSprite()->GetHeight() )*/ );
    //Vector2f targetTransform = Vector2f( circle.GetTransform()->GetPosition()/* - Vector2f( circle.GetTransform()->GetSprite()->GetWidth(), circle.GetTransform()->GetSprite()->GetHeight() )*/ );
    
    Vector2f currTransform = m_transform->GetPosition() + m_transform->GetScale();
    Vector2f targetTransform = circle.GetTransform()->GetPosition() + circle.GetTransform()->GetScale();
    float distance = Vector2f( currTransform - targetTransform ).Magnitude();

    //------------------------------- WAY IT WAS BEFORE ----------------------
    //float distance = Vector2f(
    //    m_transform->GetPosition().x - circle.GetTransform()->GetPosition().x,
    //    m_transform->GetPosition().y - circle.GetTransform()->GetPosition().y).Magnitude();

    if (distance < radiusSum)
        return true;

    return false;
}

bool CircleCollider::ToPoint(Vector2f point) noexcept
{
    Vector2f circlePos = m_transform->GetPosition();

    float distance = (circlePos - point).Magnitude();

    if (distance < m_radius)
        return true;

    return false;
}

void CircleCollider::Resolution(std::shared_ptr<Collider> collider) noexcept
{
    if (m_isTrigger)
        return;

    Vector2f position = m_transform->GetPosition();
    Vector2f newPos = position;

    switch (collider->GetColliderType())
    {
    case ColliderType::Box:
    {
        Vector2f closestPoint = ClosestSurfacePoint(collider->GetTransform()->GetPosition());
        bool changeXValue = false;
        bool changeYValue = false;

        auto boxPtr = std::dynamic_pointer_cast<BoxCollider>(collider);
        BoxCollider box = *boxPtr;

        Vector2f shiftedY = Vector2f(m_lastValidPosition.x, closestPoint.y);
        Vector2f shiftedX = Vector2f(closestPoint.x, m_lastValidPosition.y);
        changeXValue = !box.ToPoint(shiftedY);
        changeYValue = !box.ToPoint(shiftedX);

        //change gameobjects position on either the x or y axis
        if (changeXValue)
        {
            newPos.x = m_lastValidPosition.x;
        }
        //if changing the x works
        if (changeYValue)
        {
            newPos.y = m_lastValidPosition.y;
        }

        break;
    }
    case ColliderType::Circle:
    {
        /*
        //auto circlePtr = std::dynamic_pointer_cast<CircleCollider>(collider);
        //CircleCollider circle = *circlePtr;

        //Vector2f circlePosition = circle.m_transform->GetPosition();
        //Vector2f cp = Vector2f(
        //    circlePosition.x + circle.GetTransform()->GetSprite()->GetWidth() / 2.0f,
        //    circlePosition.y + circle.GetTransform()->GetSprite()->GetHeight() / 2.0f);
        //Transform tf = *circle.GetTransform();
        //circle.SetTransform(std::make_shared<Transform>(tf));
        //circle.GetTransform()->SetPosition(cp);
  
        //circle.SetRadius(circle.GetRadius() + m_radius);

        //Vector2f position = m_transform->GetPosition();
        //Vector2f sp = Vector2f(
        //    position.x + m_transform->GetSprite()->GetWidth() / 2.0f,
        //    position.y + m_transform->GetSprite()->GetHeight() / 2.0f );

        //circle.m_transform->SetPosition(circle.GetTransform()->GetPosition() + circle.GetTransform()->GetScale());
        //newPos = circle.ClosestPoint(m_transform->GetPosition());
        //break;
        */

        //------------------------------- WAY IT WAS BEFORE ----------------------
        auto circlePtr = std::dynamic_pointer_cast<CircleCollider>(collider);
        CircleCollider circle = *circlePtr;
        circle.SetRadius(circle.GetRadius() + m_radius);
        newPos = circle.ClosestPoint(m_transform->GetPosition());
        break;
    }
    }

    m_transform->SetPosition(newPos);
}

bool CircleCollider::CollisionCheck(std::shared_ptr<Collider> collider) noexcept
{
    if (CheckBlackList(collider) == true)
    {
        return false;
    }

    switch (collider->GetColliderType())
    {
    case ColliderType::Box:
        return ToBox(*std::dynamic_pointer_cast<BoxCollider>(collider));
    case ColliderType::Circle:
        return ToCircle(*std::dynamic_pointer_cast<CircleCollider>(collider));
    }

    return false;
}