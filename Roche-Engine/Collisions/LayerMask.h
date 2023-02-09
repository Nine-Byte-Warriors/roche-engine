#pragma once
#ifndef LAYERMASK_H
#define LAYERMASK_H

enum class LayerNo
{
	Decoration = 0,
	Player,
	Enemy,
	PlayerProjectile,
	EnemyProjectile,
};

//what layers can be interacted with
//from Decorations to Projectiles
struct LayerMask
{
	const int m_size = 5;
	bool m_mask[5] = { true, true, true, true, true };
	LayerMask() = default;
	LayerMask(LayerMask& layerMask)
	{
		ChangeMask(layerMask.m_mask[0],layerMask.m_mask[1],layerMask.m_mask[2],layerMask.m_mask[3], layerMask.m_mask[4]);
	};
	LayerMask(bool decorations, bool player, bool enemy, bool playerProjectiles, bool enemyProjectiles)
	{
		ChangeMask(decorations, player, enemy, playerProjectiles, enemyProjectiles);
	};

	void ChangeMask(bool decorations, bool player, bool enemy, bool playerProjectiles, bool enemyProjectiles)
	{
		m_mask[0] = decorations;
		m_mask[1] = player;
		m_mask[2] = enemy;
		m_mask[3] = playerProjectiles;
		m_mask[4] = enemyProjectiles;
	}
	void operator = (LayerMask& layerMask)
	{
		ChangeMask(layerMask.m_mask[0], layerMask.m_mask[1], layerMask.m_mask[2], layerMask.m_mask[3], layerMask.m_mask[4]);
	}
};

#endif