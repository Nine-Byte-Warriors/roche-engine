#pragma once
#ifndef UISCREEN_UI
#define UISCREEN_UI

class Graphics;
#include "Health.h"
#include "Shaders.h"
#include "Listener.h"
#include "Inventory.h"
#include "TextRenderer.h"
#include "WidgetIncludes.h"
#include "ScoreBoard.h"

class UIScreen : public Listener
{
private:
	enum class Tabs
	{
		General,
		Controls,
		Audio,
		Graphics
	} m_eTabsState = Tabs::General;

public:
	UIScreen() { AddToEvent(); }
	~UIScreen() { RemoveFromEvent(); }

	void Initialize( const Graphics& gfx, ConstantBuffer<Matrices>* mat, const std::vector<std::shared_ptr<Widget>>& widgets, Health& health );
	void Update( const float dt );
	void Draw( VertexShader& vtx, PixelShader& pix, XMMATRIX worldOrtho, TextRenderer* textRenderer );

	inline void SetWidgets( const std::vector<std::shared_ptr<Widget>>& widgets ) noexcept { m_vWidgets = widgets; }
	inline void SetScreenSize( XMFLOAT2 size ) noexcept { m_vScreenSize = size; }

private:
	void InitializeWidgets();
	void AddToEvent() noexcept;
	void RemoveFromEvent() noexcept;
	void HandleEvent( Event* event ) override;

	// Inputs
	MouseData m_mouseData;
	std::string m_sKeys;

	// Graphics
	XMFLOAT2 m_vScreenSize;
	ConstantBuffer<Matrices>* m_cbMatrices;
	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
	std::vector<std::shared_ptr<Widget>> m_vWidgets;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pContext;

	// GitHub link
	bool m_bOpen = true;
	bool m_bOpenLink = false;

	// Page slider data
	bool m_bUsingPageSlider = false;
	bool m_bUpdatePageSlider = false;
	XMFLOAT2 m_fBoxPos, m_fBoxSize;
	float m_fCurrentY;

	// Global data
	Inventory m_inventory;
	Health* m_pPlayerHealth;
	ScoreBoard m_scoreBoard;
  
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

	std::vector<std::string> m_texturesGeneralTabs =
	{
		"Resources\\Textures\\UI\\Tabs\\Gear\\GearTabIdle.png",
		"Resources\\Textures\\UI\\Tabs\\Gear\\GearTabActive.png",
		"Resources\\Textures\\UI\\Tabs\\Gear\\GearTabActive.png"
	};

	std::vector<std::string> m_texturesGraphicsTabs =
	{
		"Resources\\Textures\\UI\\Tabs\\Graphics\\GraphicsTabIdle.png",
		"Resources\\Textures\\UI\\Tabs\\Graphics\\GraphicsTabActive.png",
		"Resources\\Textures\\UI\\Tabs\\Graphics\\GraphicsTabActive.png"
	};

	std::vector<std::string> m_texturesMusicTabs =
	{
		"Resources\\Textures\\UI\\Tabs\\Music\\MusicTabIdle.png",
		"Resources\\Textures\\UI\\Tabs\\Music\\MusicTabActive.png",
		"Resources\\Textures\\UI\\Tabs\\Music\\MusicTabActive.png"
	};

	std::vector<std::string> m_texturesControlTabs =
	{
		"Resources\\Textures\\UI\\Tabs\\Control\\ControlTabIdle.png",
		"Resources\\Textures\\UI\\Tabs\\Control\\ControlTabActive.png",
		"Resources\\Textures\\UI\\Tabs\\Control\\ControlTabActive.png"
	};

	std::vector<std::string> m_texturesHearts =
	{
		"Resources\\Textures\\UI\\Hearts\\Heart-1.png",
		"Resources\\Textures\\UI\\Hearts\\Heart-2.png",
		"Resources\\Textures\\UI\\Hearts\\Heart-3.png"
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