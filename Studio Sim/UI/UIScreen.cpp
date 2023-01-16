#include "stdafx.h"
#include "UIScreen.h"
#include "Graphics.h"
#include <shellapi.h>

void UIScreen::Initialize( const Graphics& gfx, ConstantBuffer<Matrices>* mat )
{
	UIElement::Initialize( gfx, mat );
	m_image.Initialize( m_pDevice.Get(), m_pContext.Get(), *mat );
	m_energyBar.Initialize( m_pDevice.Get(), m_pContext.Get(), *mat );
	m_dataSlider.Initialize( m_pDevice.Get(), m_pContext.Get(), *mat );
	m_colourBlock.Initialize( m_pDevice.Get(), m_pContext.Get(), *mat );
	for ( unsigned int i = 0; i < ARRAYSIZE( m_buttons ); i++ )
		m_buttons[i].Initialize( m_pDevice.Get(), m_pContext.Get(), *mat );
}

void UIScreen::Update( const float dt )
{
	if ( m_bMouseLoad )
	{
		m_bMouseLoad = false;
		m_mouseData.LPress = false;
	}

	XMFLOAT2 size = { m_vScreenSize.x * 0.1f, m_vScreenSize.y * 0.1f };
	XMFLOAT2 pos = { m_vScreenSize.x * 0.075f, m_vScreenSize.y * 0.1f };
	float offset = 0.125f;

	// --- Colour Block Widgets ---
	m_colourBlock.Resolve( { 210, 210, 150 }, pos, size );
	m_colourBlock.Update( dt );
	pos.x += m_vScreenSize.x * offset;

	// --- Image  Widgets ---
	m_image.Resolve( "Resources\\Textures\\cauliflower_ss.png", pos, size );
	m_image.Update( dt );
	pos.x += m_vScreenSize.x * offset;

	// --- Data Slider Widgets ---
	m_dataSlider.Resolve( m_iSliderStart, "Resources\\Textures\\dirt.png", "Resources\\Textures\\wall.png", m_mouseData, pos, size );
	m_dataSlider.Update( dt );
	pos.x += m_vScreenSize.x * offset;

	// --- Energy Bar Widgets ---
	std::string temp = m_textures[2];
	m_textures[2] = "";
	m_energyBar.Resolve( m_textures, m_fPlayerHealth, pos, size );
	m_textures[2] = temp;
	m_energyBar.Update( dt );
	pos.x += m_vScreenSize.x * offset;

	// --- Button Widgets ---
	if ( m_buttons[0].Resolve( "", Colors::Black, m_texturesGithub, m_mouseData, pos, size ) )
		if ( !m_bOpenLink && m_bOpen )
			m_bOpenLink = true;
	m_buttons[0].Update( dt );

	if ( !m_buttons[0].GetIsPressed() )
		m_bOpen = true;

	// Github link
	if ( m_bOpenLink )
	{
		ShellExecute( 0, 0, L"https://github.com/Nine-Byte-Warriors", 0, 0, SW_SHOW );
		m_bOpenLink = false;
		m_bOpen = false;
	}
	pos.x += m_vScreenSize.x * offset;
	
	// Example button
	m_buttons[1].Resolve( "Example", Colors::Black, m_textures, m_mouseData, pos, size );
	m_buttons[1].Update( dt );
	pos.x += m_vScreenSize.x * offset;

	// Quit game
	if ( m_buttons[2].Resolve( "Exit", Colors::Black, m_textures, m_mouseData, pos, size ) )
		EventSystem::Instance()->AddEvent( EVENTID::QuitGameEvent );
	m_buttons[2].Update( dt );
}

void UIScreen::Draw( VertexShader& vtx, PixelShader& pix, XMMATRIX worldOrtho, TextRenderer* textRenderer )
{
	Shaders::BindShaders( m_pContext.Get(), vtx, pix );
	m_image.Draw( m_pDevice.Get(), m_pContext.Get(), worldOrtho );
	m_energyBar.Draw( m_pDevice.Get(), m_pContext.Get(), worldOrtho );
	m_dataSlider.Draw( m_pDevice.Get(), m_pContext.Get(), worldOrtho );
	m_colourBlock.Draw( m_pDevice.Get(), m_pContext.Get(), worldOrtho );
	for ( unsigned int i = 0; i < ARRAYSIZE( m_buttons ); i++ )
	{
		m_buttons[i].Draw( m_pDevice.Get(), m_pContext.Get(), worldOrtho, textRenderer );
		Shaders::BindShaders( m_pContext.Get(), vtx, pix );
	}
}

void UIScreen::AddToEvent() noexcept
{
	EventSystem::Instance()->AddClient( EVENTID::WindowSizeChangeEvent, this );
	EventSystem::Instance()->AddClient( EVENTID::MousePosition, this );
	EventSystem::Instance()->AddClient( EVENTID::LeftMouseClick, this );
	EventSystem::Instance()->AddClient( EVENTID::LeftMouseRelease, this );
	EventSystem::Instance()->AddClient( EVENTID::RightMouseClick, this );
	EventSystem::Instance()->AddClient( EVENTID::RightMouseRelease, this );
	EventSystem::Instance()->AddClient( EVENTID::MiddleMouseClick, this );
	EventSystem::Instance()->AddClient( EVENTID::MiddleMouseRelease, this );
	EventSystem::Instance()->AddClient( EVENTID::PlayerHealth, this );
}

void UIScreen::RemoveFromEvent() noexcept
{
	EventSystem::Instance()->RemoveClient( EVENTID::WindowSizeChangeEvent, this );
	EventSystem::Instance()->RemoveClient( EVENTID::MousePosition, this );
	EventSystem::Instance()->RemoveClient( EVENTID::LeftMouseClick, this );
	EventSystem::Instance()->RemoveClient( EVENTID::LeftMouseRelease, this );
	EventSystem::Instance()->RemoveClient( EVENTID::RightMouseClick, this );
	EventSystem::Instance()->RemoveClient( EVENTID::RightMouseRelease, this );
	EventSystem::Instance()->RemoveClient( EVENTID::MiddleMouseClick, this );
	EventSystem::Instance()->RemoveClient( EVENTID::MiddleMouseRelease, this );
	EventSystem::Instance()->RemoveClient( EVENTID::PlayerHealth, this );
}

void UIScreen::HandleEvent( Event* event )
{
	switch ( event->GetEventID() )
	{
	case EVENTID::WindowSizeChangeEvent: { m_vScreenSize = *static_cast<XMFLOAT2*>( event->GetData() ); } break;
	case EVENTID::MousePosition: { m_mouseData.Pos = *(XMFLOAT2*)event->GetData(); } break;
	case EVENTID::LeftMouseClick: { m_mouseData.LPress = true; } break;
	case EVENTID::LeftMouseRelease: { m_mouseData.LPress = false; } break;
	case EVENTID::RightMouseClick: { m_mouseData.RPress = true; } break;
	case EVENTID::RightMouseRelease: { m_mouseData.RPress = false; } break;
	case EVENTID::MiddleMouseClick: { m_mouseData.MPress = true; } break;
	case EVENTID::MiddleMouseRelease: { m_mouseData.MPress = false; } break;
	case EVENTID::PlayerHealth: { m_fPlayerHealth = *static_cast<float*>( event->GetData() ); } break;
	}
}