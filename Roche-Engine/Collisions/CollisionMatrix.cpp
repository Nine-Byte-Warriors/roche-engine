#include "stdafx.h"
#include "CollisionMatrix.h"

void CollisionMatrix::SetMatrix
(bool dD, bool dP, bool dE, bool dPj,
bool pD, bool pP, bool pE, bool pPj,
bool eD, bool eP, bool eE, bool ePj,
bool pjD, bool pjP, bool pjE, bool pjPj)
{
	SetLayerMask((int)LayerNo::Decoration, dD, dP, dE, dPj);
	SetLayerMask((int)LayerNo::Enemy, pD, pP, pE, pPj);
	SetLayerMask((int)LayerNo::Player, eD, eP, eE, ePj);
	SetLayerMask((int)LayerNo::Projectile, pjD, pjP, pjE, pjPj);
}
