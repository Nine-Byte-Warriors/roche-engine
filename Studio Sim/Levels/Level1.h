#pragma once
#ifndef LEVEL1_H
#define LEVEL1_H

#include "Camera.h"
#include "Player.h"
#include "Enemy.h"
#include "Projectile.h"
#include "TileMapDraw.h"
#include "TileMapEditor.h"
#include "LevelStateMachine.h"

#include <dxtk/SpriteFont.h>
#include <dxtk/SpriteBatch.h>

/// <summary>
/// The first level of the game.
/// Inherits from Level to render/update objects used in each level.
/// </summary>
class Level1 : public LevelContainer
{
public:
	Level1() {}

	void OnCreate() override;
	void OnSwitch() override;
	
	void BeginFrame() override;
	void RenderFrame() override;
	void EndFrame() override;

	void Update( const float dt ) override;
	void CleanUp() override {}
	
private:
	// Tile Map
	void OnCreateTileMap();
	void UpdateTileMap(const float dt);
	void RenderFrameTileMap();

	// Objects
	Camera m_camera;
	Player m_player;
	Enemy m_enemy;
	Projectile m_projectile;
	TileMapEditor m_tileMapEditor;
	ConstantBuffer<Matrices> m_cbMatrices;
	std::vector<TileMapDraw> m_tileMapDraw;

	// Fonts
	std::unique_ptr<SpriteFont> m_spriteFont;
	std::unique_ptr<SpriteBatch> m_spriteBatch;
};

#endif