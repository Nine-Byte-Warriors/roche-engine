#pragma once
#ifndef LEVEL1_H
#define LEVEL1_H

#include <dxtk/SpriteFont.h>
#include <dxtk/SpriteBatch.h>

#include "Cube.h"
#include "structures.h"
#include "ConstantBuffer.h"
#include "PostProcessing.h"
#include "LevelStateMachine.h"

/// <summary>
/// The first level of the game.
/// Inherits from Level to render/update objects used in each level.
/// </summary>
class Level1 : public LevelContainer
{
public:
	Level1( LevelStateMachine& stateMachine );

	void OnCreate() override;
	void OnSwitch() override;
	
	void BeginFrame() override;
	void RenderFrame() override;
	void EndFrame() override;

	void Update( const float dt ) override;
	void CleanUp() override {}
private:
	// Systems
	PostProcessing m_postProcessing;
	LevelStateMachine& levelStateMachine;

	// Objects
	Cube m_cube;
	ConstantBuffer<Matrices> m_cbMatrices;

	// Fonts
	std::unique_ptr<SpriteFont> m_spriteFont;
	std::unique_ptr<SpriteBatch> m_spriteBatch;
};

#endif