#pragma once
#ifndef COLLISIONMATRIX_H
#define COLLISIONMATRIX_H

#include "LayerMask.h"

//Maps which layers can interact
class CollisionMatrix
{
private:
	std::shared_ptr<std::vector<LayerMask>> m_matrix;

public:
	CollisionMatrix() { m_matrix = std::make_shared < std::vector<LayerMask>>(4); };
	//mask for each layer
	inline void SetLayerMask(int mask, LayerMask layerMask) noexcept 
	{ m_matrix->at(mask) = layerMask; }
	inline void SetLayerMask(int mask, bool decorations, bool player, bool enemy, bool playerProjectile, bool enemyProjectile) noexcept 
	{ m_matrix->at(mask).ChangeMask(decorations, player, enemy, playerProjectile, enemyProjectile); }
	LayerMask& GetLayerMask(int mask) const noexcept { return m_matrix->at(mask); }

	inline void SetMatrix(CollisionMatrix collisionMatrix)
	{
		for (int i = 0; i< collisionMatrix.m_matrix->at(i).m_size; i++)
		{
			SetLayerMask(i, collisionMatrix.m_matrix->at(i));
		}
	}
	void SetMatrix(
		bool dD, bool dP, bool dE, bool dPPj, bool dEPj,
		bool pD, bool pP, bool pE, bool pPPj, bool pEPj,
		bool eD, bool eP, bool eE, bool ePPj, bool eEPj,
		bool ppjD, bool ppjP, bool ppjE, bool ppjPPj, bool ppjEPj,
		bool epjD, bool epjP, bool epjE, bool epjPPj, bool epjEPj);

	inline bool GetElement(int mask, int element) const noexcept { return m_matrix->at(mask).m_mask[element]; };
	inline void SetElement(int mask, int element, bool value) noexcept { m_matrix->at(mask).m_mask[element] = value; };
};

#endif