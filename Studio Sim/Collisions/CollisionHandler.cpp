#include "stdafx.h"
#include "CollisionHandler.h"

void CollisionHandler::AddCollider(std::shared_ptr<Collider> collider)
{
    m_colliders.push_back(collider.get());
}

void CollisionHandler::AddCollider(std::vector<std::shared_ptr<Collider>>& colliders)
{
    for (int i = 0; i < colliders.size(); i++)
    {
        m_colliders.push_back(colliders[i].get());
    }
}

bool CollisionHandler::PointToBox(Vector2f point, BoxCollider* box)
{
    float boxHalfWidth = (box->GetWidth() / 2);
    float boxHalfHeight = (box->GetHeight() / 2);

    Vector2f boxMin(box->GetTransform()->GetPosition().x - boxHalfWidth, box->GetTransform()->GetPosition().y - boxHalfHeight);
    Vector2f boxMax(box->GetTransform()->GetPosition().x + boxHalfWidth, box->GetTransform()->GetPosition().y + boxHalfHeight);

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

bool CollisionHandler::PointToCircle(Vector2f point, CircleCollider* circle)
{
    Vector2f circlePos = circle->GetTransform()->GetPosition();
    
    int distance = (circlePos - point).Magnitude();

    if (distance < circle->GetRadius())
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool CollisionHandler::CollisionCheck(Collider* collider1, Collider* collider2)
{
    if (collider1->GetColliderType() == ColliderType::Box && collider2->GetColliderType() == ColliderType::Box)
    {
        return BoxToBox((BoxCollider*)collider1, (BoxCollider*)collider2);
    }
    else if (collider1->GetColliderType() == ColliderType::Circle && collider2->GetColliderType() == ColliderType::Circle)
    {
        return CircleToCircle((CircleCollider*)collider1, (CircleCollider*)collider2);
    }
    else if (collider1->GetColliderType() == ColliderType::Box && collider2->GetColliderType() == ColliderType::Circle)
    {
        return CircleToBox((CircleCollider*)collider2, (BoxCollider*)collider1);
    }
    else if (collider1->GetColliderType() == ColliderType::Circle && collider2->GetColliderType() == ColliderType::Box)
    {
        return CircleToBox((CircleCollider*)collider1, (BoxCollider*)collider2);
    }

    return false;
}

void CollisionHandler::CollisionCheckAll()
{
    bool isCollision;
    for (int i = 0; i < m_colliders.size(); i++)
    {
        isCollision = false;
        for (int n = 0; n < m_colliders.size(); n++)
        {
            if (n == i)
            {
                continue;
            }

            bool isIntersection = CollisionCheck(m_colliders[i], m_colliders[n]);
            //bool isIntersection = m_colliders[i]->CollisionCheck(m_colliders[n]);

            if (isIntersection)
            {
                isCollision = true;
                //m_colliders[i]->GetTransform()->SetPosition(m_colliders[n]->GetLastValidPosition());
                m_colliders[i]->Resolution(m_colliders[n]);
                //Resolution(m_colliders[i], m_colliders[n]);
            }
        }
        if (isCollision == false)
        {
            m_colliders[i]->UpdateLastValidPosition();
        }
    }
}

void CollisionHandler::Resolution(Collider* collider1, Collider* collider2)
{
    if(collider1->GetIsTrigger() == true || collider2->GetIsTrigger() == true)
    {
        return;
    }

    //Vector2f closestPoint1;
    //Vector2f closestPoint2;
    //if (collider1->GetColliderType() == ColliderType::Box && collider2->GetColliderType() == ColliderType::Box)
    //{
    //    BoxCollider* box1 = (BoxCollider*)collider1;
    //    BoxCollider* box2 = (BoxCollider*)collider2;
    //    closestPoint1 = box1->ClosestPoint(box2->GetTransform()->GetPosition());
    //    closestPoint2 = box2->ClosestPoint(box1->GetTransform()->GetPosition());
    //}
    //else if (collider1->GetColliderType() == ColliderType::Circle && collider2->GetColliderType() == ColliderType::Circle)
    //{
    //    CircleCollider* circle1 = (CircleCollider*)collider1;
    //    CircleCollider* circle2 = (CircleCollider*)collider2;
    //    closestPoint1 = circle1->ClosestPoint(circle2->GetTransform()->GetPosition());
    //    closestPoint2 = circle2->ClosestPoint(circle1->GetTransform()->GetPosition());
    //}
    //else if (collider1->GetColliderType() == ColliderType::Box && collider2->GetColliderType() == ColliderType::Circle)
    //{
    //    BoxCollider* box = (BoxCollider*)collider1;
    //    CircleCollider* circle = (CircleCollider*)collider2;
    //    closestPoint1 = box->ClosestPoint(circle->GetTransform()->GetPosition());
    //    closestPoint2 = circle->ClosestPoint(box->GetTransform()->GetPosition());
    //}
    //else if (collider1->GetColliderType() == ColliderType::Circle && collider2->GetColliderType() == ColliderType::Box)
    //{
    //    CircleCollider* circle = (CircleCollider*)collider1;
    //    BoxCollider* box = (BoxCollider*)collider2;
    //    closestPoint1 = circle->ClosestPoint(box->GetTransform()->GetPosition());
    //    closestPoint2 = box->ClosestPoint(circle->GetTransform()->GetPosition());
    //}
    
    //Vector2f intersectDistance = closestPoint2 - closestPoint1;
    //collider1->GetTransform()->SetPosition((collider1->GetTransform()->GetPosition() + intersectDistance));
    
    Vector2f closestPoint = ClosestPoint(collider1,collider2->GetTransform()->GetPosition());

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
        BoxCollider* box = (BoxCollider*) collider2;
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

Vector2f CollisionHandler::ClosestPoint(Collider* collider1, Vector2f collider2)
{
    ColliderType collider1Type = collider1->GetColliderType();

    BoxCollider* box;
    CircleCollider* circle;

    switch (collider1Type)
    {
    case ColliderType::Box:
        box = (BoxCollider*)collider1;
        return box->ClosestPoint(collider2);
    case ColliderType::Circle:
        circle = (CircleCollider*) collider1;
        return circle->ClosestPoint(collider2);
    }
}

void CollisionHandler::Update()
{
    CollisionCheckAll();
}

