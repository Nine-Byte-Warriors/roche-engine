#pragma once
#include <memory>
#include <vector>
#include "LayerMask.h"

//Maps which layers can interact
class CollisionMatrix
{
	std::shared_ptr<std::vector<LayerMask>> m_matrix;

public:
	CollisionMatrix() { m_matrix = std::make_shared < std::vector<LayerMask>>(4); };
	//mask for each layer
	void SetLayerMask(int mask, LayerMask layerMask) { m_matrix->at(mask).ChangeMask(layerMask.m_mask[0], layerMask.m_mask[1], layerMask.m_mask[2], layerMask.m_mask[3]); }
	void SetLayerMask(int mask, bool decorations, bool player, bool enemy, bool projectiles) { m_matrix->at(mask).ChangeMask(decorations, player, enemy, projectiles); }
	LayerMask& GetLayerMask(int mask) { return m_matrix->at(mask); }

	void SetMatrix(CollisionMatrix collisionMatrix)
	{
		for (int i = 0; i< collisionMatrix.m_matrix->at(i).m_size; i++)
		{
			SetLayerMask(i, collisionMatrix.m_matrix->at(i));
		}
	}
	void SetMatrix(bool dD, bool dP, bool dE, bool dPj,
					bool pD, bool pP, bool pE, bool pPj,
					bool eD, bool eP, bool eE, bool ePj,
					bool pjD, bool pjP, bool pjE, bool pjPj);

	bool GetElement(int mask, int element) { return m_matrix->at(mask).m_mask[element]; };
	void SetElement(int mask, int element, bool value) { m_matrix->at(mask).m_mask[element] = value; };
};