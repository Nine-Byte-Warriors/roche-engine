#pragma once

#include <vector>
#include "Vector2f.h"

const enum class ColliderType
{
    None,
    Box,
    Circle
};

class Collider
{
public:
    Vector2f m_position = Vector2f(0,0);

    ColliderType m_type = ColliderType::None;
};

//AABB
class BoxCollider : public Collider
{
public:
    BoxCollider() { m_type = ColliderType::Box; };
    BoxCollider(int x, int y, int width, int height) : m_w(width), m_h(height) { m_position = Vector2f(x, y); }
public:
    
    //position from bottom left
    float m_w = 0;
    float m_h = 0;
};

class CircleCollider : public Collider
{
public:
    CircleCollider() { m_type = ColliderType::Circle; };
    CircleCollider(int x, int y, float radius) : m_radius(radius) { m_position = Vector2f(x, y); }
public:
    float m_radius = 0;
};
