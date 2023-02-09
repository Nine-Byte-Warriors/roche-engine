#include "stdafx.h"
#include "CollisionMatrix.h"

void CollisionMatrix::SetMatrix
(bool dD, bool dP, bool dE, bool dPPj, bool dEPj,
bool pD, bool pP, bool pE, bool pPPj, bool pEPj,
bool eD, bool eP, bool eE, bool ePPj, bool eEPj,
bool ppjD, bool ppjP, bool ppjE, bool ppjPPj, bool ppjEPj,
bool epjD, bool epjP, bool epjE, bool epjPPj, bool epjEPj)
{
	SetLayerMask((int)LayerNo::Decoration, dD, dP, dE, dPPj,dEPj);
	SetLayerMask((int)LayerNo::Enemy, pD, pP, pE, pPPj, pEPj);
	SetLayerMask((int)LayerNo::Player, eD, eP, eE, ePPj, eEPj);
	SetLayerMask((int)LayerNo::PlayerProjectile, ppjD, ppjP, ppjE, ppjPPj, ppjEPj);
	SetLayerMask((int)LayerNo::EnemyProjectile, epjD, epjP, epjE, epjPPj, epjEPj);
}
