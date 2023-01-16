#include "stdafx.h"
#include "CollisionHandler.h"

void CollisionHandler::AddCollider(Collider& collider)
{
    m_colliders.push_back(&collider);
}

void CollisionHandler::AddCollider(std::vector<Collider>& colliders)
{
    for (auto collider:colliders)
    {
        m_colliders.push_back(&collider);
    }
}

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
    Vector2f closestPoint;
    closestPoint.x = Clamp(box->GetTransform()->GetPosition().x, box->GetTransform()->GetPosition().x + box->GetWidth(), circle->GetTransform()->GetPosition().x);
    closestPoint.y = Clamp(box->GetTransform()->GetPosition().y, box->GetTransform()->GetPosition().y + box->GetHeight(), circle->GetTransform()->GetPosition().y);
    Vector2f circlePos(circle->GetTransform()->GetPosition().x, circle->GetTransform()->GetPosition().y);

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
                Resolution(*m_colliders[i], *m_colliders[n]);
            }
        }
        if (!isCollision)
        {
            m_colliders[i]->UpdateLastValidPosition();
        }
        startIndex++;
    } 
}

void CollisionHandler::Resolution(Collider& collider1, Collider& collider2)
{
    collider1.GetTransform()->SetPosition(collider1.GetLastValidPosition());
    collider2.GetTransform()->SetPosition(collider2.GetLastValidPosition());
}

void CollisionHandler::Update()
{
    CheckAll();
}

