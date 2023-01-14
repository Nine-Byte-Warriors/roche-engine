#pragma once
#ifndef MAINMENU_UI
#define MAINMENU_UI

#include "UI.h"

class MainMenu_UI : public UI
{
public:
	MainMenu_UI() {}
	~MainMenu_UI() { RemoveFromEvent(); }

	void Initialize( const Graphics& gfx, ConstantBuffer<Matrices>* mat );
	void Update( const float dt );
	void Draw( XMMATRIX worldOrtho );
	
	void TextLoad();
	void HandleEvent( Event* event ) override;
private:
	void AddToEvent();
	void RemoveFromEvent();

	void MenuButtons();
	void LinkButtons();

	// MainMenu
	Image_Widget m_titlecard;
	ColourBlock_Widget m_mainMenuBackground;
	Button_Widget<std::string> m_mainMenuButtons[5];
	std::unordered_map<std::string, std::string> m_mLoadedTextMap;
	
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