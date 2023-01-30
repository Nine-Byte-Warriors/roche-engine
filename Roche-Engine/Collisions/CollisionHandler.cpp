#include "stdafx.h"
#include "CollisionHandler.h"

CollisionHandler::CollisionHandler(CollisionMatrix collisionMatrix)
{
    m_collisionMatrix.SetMatrix(collisionMatrix);
}

void CollisionHandler::AddCollider(std::shared_ptr<Collider> collider)
{
    m_colliders.push_back(collider);
}

void CollisionHandler::AddCollider(std::vector<std::shared_ptr<Collider>> colliders)
{
    for (int i = 0; i < colliders.size(); i++)
    {
        m_colliders.push_back(colliders[i]);
    }
}

void CollisionHandler::SetMatrix(bool dD, bool dP, bool dE, bool dPj,
    bool pD, bool pP, bool pE, bool pPj,
    bool eD, bool eP, bool eE, bool ePj,
    bool pjD, bool pjP, bool pjE, bool pjPj)
{
    m_collisionMatrix.SetMatrix(dD, dP, dE, dPj,
        pD, pP, pE, pPj,
        eD, eP, eE, ePj,
        pjD, pjP, pjE, pjPj);
}

//bool CollisionHandler::BoxToBox(BoxCollider* box1, BoxCollider *box2)
void CollisionHandler::RemoveAllColliders()
{
    m_colliders.clear();
}

//int Clamp(int min, int max, int value)
//{
//    float box1HalfWidth = (box1->GetWidth() / 2);
//    float box1HalfHeight = (box1->GetHeight() / 2);
//
//    float box2HalfWidth = (box2->GetWidth() / 2);
//    float box2HalfHeight = (box2->GetHeight() / 2);
//
//    Vector2f box1Min(box1->GetTransform()->GetPosition().x - box1HalfWidth, box1->GetTransform()->GetPosition().y - box1HalfHeight);
//    Vector2f box1Max(box1->GetTransform()->GetPosition().x + box1HalfWidth, box1->GetTransform()->GetPosition().y + box1HalfHeight);
//
//    Vector2f box2Min(box2->GetTransform()->GetPosition().x - box1HalfWidth, box2->GetTransform()->GetPosition().y - box2HalfHeight);
//    Vector2f box2Max(box2->GetTransform()->GetPosition().x + box1HalfWidth, box2->GetTransform()->GetPosition().y + box2HalfHeight);
//
////if (box1->GetTransform()->GetPosition().x < box2->GetTransform()->GetPosition().x + box2->GetWidth() / 2 &&
////    box1->GetTransform()->GetPosition().x + box1->GetWidth() / 2 > box2->GetTransform()->GetPosition().x &&
////    box1->GetTransform()->GetPosition().y < box2->GetTransform()->GetPosition().y + box2->GetHeight() / 2 &&
////    box1->GetTransform()->GetPosition().y + box2->GetHeight() / 2 > box2->GetTransform()->GetPosition().y)
////{
////    return true;
////}
////else
////{
////    return false;
////}
//
//    if (box1Min.x < box2Max.x &&
//        box1Max.x > box2Min.x &&
//        box1Min.y < box2Max.y &&
//        box1Max.y > box2Min.y)
//    {
//        return true;
//    }
//    else
//    {
//        return false;
//    }
//}
//bool CollisionHandler::CircleToBox(CircleCollider* circle, BoxCollider* box)
//{
//    Vector2f circlePos = circle->GetTransform()->GetPosition();
//    Vector2f closestPoint = box->ClosestPoint(circlePos);
//
//    int distance = (circlePos - closestPoint).Magnitude();
//
//    if (distance < circle->GetRadius())
//    {
//        return true;
//    }
//    else
//    {
//        return false;
//    }
//}
//bool CollisionHandler::CircleToBox(BoxCollider* box, CircleCollider* circle)
//{
//    Vector2f circlePos = circle->GetTransform()->GetPosition();
//    Vector2f closestPoint = box->ClosestPoint(circlePos);
//
//    int distance = (circlePos - closestPoint).Magnitude();
//
//    if (distance < circle->GetRadius())
//    {
//        return true;
//    }
//    else
//    {
//        return false;
//    }
//}
//
//bool CollisionHandler::CircleToCircle(CircleCollider* circle1, CircleCollider* circle2)
//{
//    int radiusSum = circle1->GetRadius() + circle2->GetRadius();
//    int distance = Vector2f(circle1->GetTransform()->GetPosition().x - circle2->GetTransform()->GetPosition().x,
//        circle1->GetTransform()->GetPosition().y - circle2->GetTransform()->GetPosition().y).Magnitude();
//
//    if (distance < radiusSum)
//    {
//        return true;
//    }
//    else
//    {
//        return false;
//    }
//}
//
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
//
//bool CollisionHandler::CollisionCheck(Collider* collider1, Collider* collider2)
//{
//    if (collider1->GetColliderType() == ColliderType::Box && collider2->GetColliderType() == ColliderType::Box)
//    {
//        return BoxToBox((BoxCollider*)collider1, (BoxCollider*)collider2);
//    }
//    else if (collider1->GetColliderType() == ColliderType::Circle && collider2->GetColliderType() == ColliderType::Circle)
//    {
//        return CircleToCircle((CircleCollider*)collider1, (CircleCollider*)collider2);
//    }
//    else if (collider1->GetColliderType() == ColliderType::Box && collider2->GetColliderType() == ColliderType::Circle)
//    {
//        return CircleToBox((CircleCollider*)collider2, (BoxCollider*)collider1);
//    }
//    else if (collider1->GetColliderType() == ColliderType::Circle && collider2->GetColliderType() == ColliderType::Box)
//    {
//        return CircleToBox((CircleCollider*)collider1, (BoxCollider*)collider2);
//    }
//
//    return false;
//}

void CollisionHandler::CollisionCheckAll()
{
    int startIndex = 0;
    bool isCollision;
    bool layersInteract;
    bool collidersInteract;
    for (int i = 0; i < m_colliders.size(); i++)
    {
        isCollision = false;
        for (int n = 0; n < m_colliders.size(); n++)
        {
            layersInteract = m_collisionMatrix.GetElement((int)m_colliders[i]->GetLayer(), (int)m_colliders[n]->GetLayer());
            collidersInteract = m_colliders[i]->GetLayerMask().m_mask[(int)m_colliders[n]->GetLayer()];

            if (n == i || layersInteract == false || collidersInteract == false)
            {
                continue;
            }

            bool isIntersection = m_colliders[i]->CollisionCheck(m_colliders[n].get());

            if (isIntersection)
            {
                isCollision = true;
                std::shared_ptr<Collider> collider1 = std::make_shared<Collider>(*m_colliders[i]);
                std::shared_ptr<Collider> collider2 = std::make_shared<Collider>(*m_colliders[n]);
                m_colliders[i]->LogCollision(collider2);
                m_colliders[n]->LogCollision(collider1);

                //m_colliders[i]->Resolution(m_colliders[n].get());
                Resolution(m_colliders[i].get(), m_colliders[n].get());
            }
        }
        if (!isCollision)
        {
            m_colliders[i]->UpdateLastValidPosition();
        }
       // startIndex++;
    }
}

void CollisionHandler::Resolution(Collider* collider1, Collider* collider2)
{
    if (collider1->GetIsTrigger() == true || collider2->GetIsTrigger() == true)
    {
        return;
    }

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
        circle = (CircleCollider*)collider1;
        return circle->ClosestPoint(collider2);
    }
}

void CollisionHandler::Update()
{
    CollisionCheckAll();
    for (auto& col : m_colliders)
    {
        col->Update();
    }
}
