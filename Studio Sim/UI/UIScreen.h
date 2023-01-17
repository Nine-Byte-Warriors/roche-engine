#pragma once
#ifndef MAINMENU_UI
#define MAINMENU_UI

class Graphics;
#include "Shaders.h"
#include "UIElement.h"

class UIScreen : public UIElement
{
public:
	UIScreen() { AddToEvent(); }
	~UIScreen() { RemoveFromEvent(); }

	void Initialize( const Graphics& gfx, ConstantBuffer<Matrices>* mat ) override;
	void Update( const float dt ) override;
	void Draw( VertexShader& vtx, PixelShader& pix, XMMATRIX worldOrtho, TextRenderer* textRenderer ) override;
	
	void AddToEvent() noexcept;
	void RemoveFromEvent() noexcept;
	void HandleEvent( Event* event ) override;
private:
	// Main Menu
	Image_Widget m_image;
	Button_Widget m_buttons[3];
	EnergyBar_Widget m_energyBar;
	DataSlider_Widget m_dataSlider;
	ColourBlock_Widget m_colourBlock;
	
	// GitHub link
	bool m_bOpen = true;
	bool m_bOpenLink = false;

	bool m_bMouseLoad = true;
	int m_iSliderStart = 50;
	float m_fPlayerHealth;
	UINT32 m_uLevelTo = 0;

	// Button state textures
	std::vector<std::string> m_textures =
	{
		"Resources\\Textures\\UI\\Button\\ButtonIdle.png",
		"Resources\\Textures\\UI\\Button\\ButtonActive.png",
		"Resources\\Textures\\UI\\Button\\ButtonActive.png"
	};

	// Logo from https://www.flaticon.com/free-icon/github_5968896
	std::vector<std::string> m_texturesGithub =
	{
		"Resources\\Textures\\github_logo.png",
		"Resources\\Textures\\github_logo.png",
		"Resources\\Textures\\github_logo.png"
	};
};

#endif