#pragma once
#ifndef MAINMENU_UI
#define MAINMENU_UI

class Graphics;
#include "Shaders.h"
#include "Listener.h"
#include "TextRenderer.h"
#include "WidgetIncludes.h"

class UIScreen : public Listener
{
public:
	UIScreen() { AddToEvent(); }
	~UIScreen() { RemoveFromEvent(); }

	void Initialize( const Graphics& gfx, ConstantBuffer<Matrices>* mat );
	void Update( const float dt );
	void Draw( VertexShader& vtx, PixelShader& pix, XMMATRIX worldOrtho, TextRenderer* textRenderer );

	inline void SetCB( ConstantBuffer<Matrices>* mat ) noexcept { m_cbMatrices = mat; }
	inline void SetScreenSize( XMFLOAT2 size ) noexcept { m_vScreenSize = size; }
	
	void AddToEvent() noexcept;
	void RemoveFromEvent() noexcept;
	void HandleEvent( Event* event ) override;
private:
	// Main Menu
	Image_Widget m_image;
	Input_Widget m_inputBox;
	Button_Widget m_buttons[3];
	DropDown_Widget m_dropDown;
	EnergyBar_Widget m_energyBar;
	DataSlider_Widget m_dataSlider;
	PageSlider_Widget m_pageSlider;
	ColourBlock_Widget m_colourBlock;

	// Inputs
	MouseData m_mouseData;
	std::string m_sKeys;

	// Graphics
	XMFLOAT2 m_vScreenSize;
	ConstantBuffer<Matrices>* m_cbMatrices;

	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pContext;
	
	// GitHub link
	bool m_bOpen = true;
	bool m_bOpenLink = false;

	// Page slider data
	bool m_bLoadFlag = true;
	XMFLOAT2 m_fBoxPos, m_fBoxSize;
	float m_fCurrentY, m_fCurrentPY;

	// Widget data
	float m_fPlayerHealth;
	bool m_bMouseLoad = true;
	int m_iSliderStart = 50;
	UINT32 m_uLevelTo = 0;

	// Button state textures
	std::vector<std::string> m_textures =
	{
		"Resources\\Textures\\UI\\Button\\ButtonIdle.png",
		"Resources\\Textures\\UI\\Button\\ButtonActive.png",
		"Resources\\Textures\\UI\\Button\\ButtonActive.png"
	};

	std::vector<std::string> m_texturesDD =
	{
		"Resources\\Textures\\UI\\Drop-Down\\DropDownMain.png",
		"Resources\\Textures\\UI\\Drop-Down\\DropDownOther.png",
		"Resources\\Textures\\UI\\Drop-Down\\DropDownOther.png"
	};

	std::vector<std::string> m_texturesDDButton =
	{
		"Resources\\Textures\\UI\\Drop-Down\\DropDownButtonIdle.png",
		"Resources\\Textures\\UI\\Drop-Down\\DropDownButtonHover.png",
		"Resources\\Textures\\UI\\Drop-Down\\DropDownButtonHover.png"
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