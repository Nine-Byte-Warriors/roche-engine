#pragma once
#ifndef LEVEL1_H
#define LEVEL1_H

#include "Cube.h"
#include "Agent.h"
#include "Player.h"
#include "Camera2D.h"
#include "PostProcessing.h"
#include "LevelStateMachine.h"
#include "TileMapEditor.h"

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
	// Objects
	Cube m_cube;
	Agent m_enemy;
	Player m_player;
	Camera2D m_camera2D;
	
	bool m_bUseCustomPP;
	TileMapEditor m_tileMapEditor;
	PostProcessing m_postProcessing;
	
	ConstantBuffer<Matrices> m_cbMatrices;
	ConstantBuffer<Matrices2D> m_cbMatrices2D;

	// Fonts
	std::unique_ptr<SpriteFont> m_spriteFont;
	std::unique_ptr<SpriteBatch> m_spriteBatch;
};

#endif