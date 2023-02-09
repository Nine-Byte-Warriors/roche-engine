#pragma once
#ifndef LAYERMASK_H
#define LAYERMASK_H

enum class LayerNo
{
	Decoration = 0,
	Player,
	Enemy,
	Projectile
};

//what layers can be interacted with
//from Decorations to Projectiles
struct LayerMask
{
	const int m_size = 4;
	bool m_mask[4] = { true, true, true, true };
	LayerMask() = default;
	LayerMask(LayerMask& layerMask)
	{
		ChangeMask(layerMask.m_mask[0],layerMask.m_mask[1],layerMask.m_mask[2],layerMask.m_mask[3]);
	};
	LayerMask(bool decorations, bool player, bool enemy, bool projectiles)
	{
		ChangeMask(decorations, player, enemy, projectiles);
	};

	void ChangeMask(bool decorations, bool player, bool enemy, bool projectiles)
	{
		m_mask[0] = decorations;
		m_mask[1] = player;
		m_mask[2] = enemy;
		m_mask[3] = projectiles;
	}
	void operator = (LayerMask& layerMask)
	{
		ChangeMask(layerMask.m_mask[0], layerMask.m_mask[1], layerMask.m_mask[2], layerMask.m_mask[3]);
	}
};

#endif