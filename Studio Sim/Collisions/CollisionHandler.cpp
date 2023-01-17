#include "stdafx.h"
#include "CollisionHandler.h"

void CollisionHandler::AddCollider(std::shared_ptr<Collider> collider)
{
    m_colliders.push_back(collider.get());
}

void CollisionHandler::AddCollider(std::vector<Collider>& colliders)
{
    for (int i = 0; i < colliders.size(); i++)
    {
        m_colliders.push_back(&colliders[i]);
    }
}

//int Clamp(int min, int max, int value)
//{
//    if (value > max)
//    {
//        return max;
//    }
//    else if (value < min)
//    {
//        return min;
//    }
//    else
//    {
//        return value;
//    }
//}
bool CollisionHandler::BoxToBox(BoxCollider* box1, BoxCollider *box2)
{
    if (box1->GetTransform()->GetPosition().x < box2->GetTransform()->GetPosition().x + box2->GetWidth() &&
        box1->GetTransform()->GetPosition().x + box1->GetWidth() > box2->GetTransform()->GetPosition().x &&
        box1->GetTransform()->GetPosition().y < box2->GetTransform()->GetPosition().y + box2->GetHeight() &&
        box1->GetTransform()->GetPosition().y + box2->GetHeight() > box2->GetTransform()->GetPosition().y)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool CollisionHandler::CircleToBox(CircleCollider *circle, BoxCollider *box)
{
    //closestPoint.x = Clamp(box->GetTransform()->GetPosition().x, box->GetTransform()->GetPosition().x + box->GetWidth(), circle->GetTransform()->GetPosition().x);
    //closestPoint.y = Clamp(box->GetTransform()->GetPosition().y, box->GetTransform()->GetPosition().y + box->GetHeight(), circle->GetTransform()->GetPosition().y);
    
    Vector2f circlePos(circle->GetTransform()->GetPosition().x, circle->GetTransform()->GetPosition().y);
    Vector2f closestPoint = box->ClosestPoint(circlePos);

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
bool CollisionHandler::CircleToCircle(CircleCollider *circle1, CircleCollider *circle2)
{
    int radiusSum = circle1->GetRadius() + circle2->GetRadius();
    int distance = Vector2f(circle1->GetTransform()->GetPosition().x - circle2->GetTransform()->GetPosition().x,
                            circle1->GetTransform()->GetPosition().y - circle2->GetTransform()->GetPosition().y).Magnitude();

    if (distance < radiusSum)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool CollisionHandler::CollisionChecker(Collider* collider1, Collider* collider2)
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
        return CircleToBox( (CircleCollider*)collider1, (BoxCollider*)collider2);
    }

    return false;
}

void CollisionHandler::CheckAll()
{
    int startIndex = 0;
    bool isCollision;
    for (int i = 0; i < m_colliders.size(); i++)
    {
        isCollision = false;
        for (int n = startIndex; n < m_colliders.size(); n++)
        {
            if (n == i)
            {
                continue;
            }

            bool isIntersection = CollisionChecker(m_colliders[i], m_colliders[n]);

            if (isIntersection)
            {
                isCollision = true;
                Resolution(m_colliders[i], m_colliders[n]);
            }
        }
        if (!isCollision)
        {
            m_colliders[i]->UpdateLastValidPosition();
        }
        startIndex++;
    } 
}

void CollisionHandler::Resolution(Collider* collider1, Collider* collider2)
{   
    Vector2f closestPoint1;
    Vector2f closestPoint2;
    if (collider1->GetColliderType() == ColliderType::Box && collider2->GetColliderType() == ColliderType::Box)
    {
        BoxCollider* box1 = (BoxCollider*)collider1;
        BoxCollider* box2 = (BoxCollider*)collider2;
        closestPoint1 = box1->ClosestPoint(box2->GetTransform()->GetPosition());
        closestPoint2 = box2->ClosestPoint(box1->GetTransform()->GetPosition());
    }
    else if (collider1->GetColliderType() == ColliderType::Circle && collider2->GetColliderType() == ColliderType::Circle)
    {
        CircleCollider* circle1 = (CircleCollider*)collider1;
        CircleCollider* circle2 = (CircleCollider*)collider2;
        closestPoint1 = circle1->ClosestPoint(circle2->GetTransform()->GetPosition());
        closestPoint2 = circle2->ClosestPoint(circle1->GetTransform()->GetPosition());
    }
    else if (collider1->GetColliderType() == ColliderType::Box && collider2->GetColliderType() == ColliderType::Circle)
    {
        BoxCollider* box = (BoxCollider*)collider1;
        CircleCollider* circle = (CircleCollider*)collider2;
        closestPoint1 = box->ClosestPoint(circle->GetTransform()->GetPosition());
        closestPoint2 = circle->ClosestPoint(box->GetTransform()->GetPosition());
    }
    else if (collider1->GetColliderType() == ColliderType::Circle && collider2->GetColliderType() == ColliderType::Box)
    {
        CircleCollider* circle = (CircleCollider*)collider1;
        BoxCollider* box = (BoxCollider*)collider2;
        closestPoint1 = circle->ClosestPoint(box->GetTransform()->GetPosition());
        closestPoint2 = box->ClosestPoint(circle->GetTransform()->GetPosition());
    }
    
    Vector2f intersectDistance = closestPoint2 - closestPoint1;
    collider1->GetTransform()->SetPosition((collider1->GetTransform()->GetPosition() + intersectDistance));
}

void CollisionHandler::Update()
{
    CheckAll();
}

