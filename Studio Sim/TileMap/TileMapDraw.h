#pragma once
#ifndef TILEMAPDRAW_H
#define TILEMAPDRAW_H

#include "Sprite.h"
#include "Transform.h"

class Graphics;

class TileMapDraw
{
public:
	TileMapDraw();
	void Initialize(const Graphics& gfx, ConstantBuffer<Matrices>& mat, const std::string& path);
	void Update(const float dt);

	inline std::shared_ptr<Sprite> GetSprite() const noexcept { return m_sprite; }
	inline std::shared_ptr<Transform> GetTransform() const noexcept { return m_transform; }
private:
	std::shared_ptr<Sprite> m_sprite;
	std::shared_ptr<Transform> m_transform;
};

#endif