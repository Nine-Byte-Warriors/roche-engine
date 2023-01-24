#pragma once

enum class LayerNo
{
	Decorations = 0,
	Player,
	Enemy,
	Projectiles
};

//what layers can be interacted with
//from Decorations to Projectiles
struct LayerMask
{
	bool mask[4] = { true, true, true, true };
	LayerMask() = default;
	LayerMask(const LayerMask& layerMask) = default;
	LayerMask(bool decorations, bool player, bool enemy, bool projectiles)
	{
		ChangeMask(decorations, player, enemy, projectiles);
	};

	void ChangeMask(bool decorations, bool player, bool enemy, bool projectiles)
	{
		mask[0] = decorations;
		mask[1] = player;
		mask[2] = enemy;
		mask[3] = projectiles;
	}
};