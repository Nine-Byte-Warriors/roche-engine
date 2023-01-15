#pragma once
#ifndef MAINMENU_UI
#define MAINMENU_UI

class Graphics;

#include "UIElement.h"

class UIScreen : public UIElement
{
public:
	UIScreen() {}
	~UIScreen() { RemoveFromEvent(); }

	void Initialize( const Graphics& gfx, ConstantBuffer<Matrices>* mat ) override;
	void Update( const float dt ) override;
	void Draw( XMMATRIX worldOrtho, TextRenderer* textRenderer ) override;
	
	void AddToEvent() noexcept;
	void RemoveFromEvent() noexcept;
	void HandleEvent( Event* event ) override;
private:
	// Main Menu
	Image_Widget m_titlecard;
	Button_Widget m_mainMenuButtons[2];
	ColourBlock_Widget m_mainMenuBackground;
	
	// GitHub link
	bool m_bOpen = true;
	bool m_bOpenLink = false;

	bool m_bMouseLoad = true;
	UINT32 m_uLevelTo = 0;

	// Button state textures
	std::vector<std::string> m_buttonTexturesMain =
	{
		"Resources\\Textures\\dirt.png",
		"Resources\\Textures\\grass.png",
		"Resources\\Textures\\wall.png"
	};

	// Logo from https://www.flaticon.com/free-icon/github_5968896
	std::vector<std::string> m_buttonTexturesGithub =
	{
		"Resources\\Textures\\github_logo.png",
		"Resources\\Textures\\github_logo.png",
		"Resources\\Textures\\github_logo.png"
	};
};

#endif