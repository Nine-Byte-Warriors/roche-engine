#pragma once
#include "Collider.h"

class Collision
{
public:
    static const bool BoxToBox(BoxCollider box1, BoxCollider box2);
    static const bool CircleToBox(CircleCollider circle, BoxCollider box);
    static const bool CircleToCircle(CircleCollider circle1, CircleCollider circle2);
};

