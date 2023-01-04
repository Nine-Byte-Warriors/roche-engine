#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

#include <dxtk/SpriteFont.h>
#include <dxtk/SpriteBatch.h>
#include "DDSTextureLoader.h"
#include "Resource.h"

#include "Cube.h"
#include "Input.h"
#include "Timer.h"
#include "Shaders.h"
#include "structures.h"
#include "ImGuiManager.h"
#include "ConstantBuffer.h"
#include "PostProcessing.h"
#include "WindowContainer.h"

class Application : public WindowContainer
{
public:
	bool Initialize( HINSTANCE hInstance, int width, int height );
	void CleanupDevice();

	bool ProcessMessages() noexcept;
	void Update();
	void Render();
private:
	// Objects
	Cube m_cube;
	Camera m_camera;
	ImGuiManager m_imgui;
	
	// Systems
	PostProcessing m_postProcessing;

	// Data
	Timer m_timer;
	Input m_input;
	
	ConstantBuffer<Matrices> m_cbMatrices;
	std::unique_ptr<SpriteFont> m_spriteFont;
	std::unique_ptr<SpriteBatch> m_spriteBatch;
};

#endif