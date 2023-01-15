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
	bool m_bIsSettings;

	// Button state textures
	std::vector<std::string> m_buttonTexturesMain =
	{
		"Resources\\Textures\\UI_Buttons\\Button_1_Down.dds",
		"Resources\\Textures\\UI_Buttons\\Button_1_Hover.dds",
		"Resources\\Textures\\UI_Buttons\\Button_1_Up.dds"
	};

	// Logo from https://github.com/logos
	std::vector<std::string> m_buttonTexturesGithub =
	{
		"Resources\\Textures\\UI_Buttons\\GitHubLogo.png",
		"Resources\\Textures\\UI_Buttons\\GitHubLogo.png",
		"Resources\\Textures\\UI_Buttons\\GitHubLogo.png"
	};
};

#endif