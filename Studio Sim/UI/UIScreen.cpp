#include "stdafx.h"
#include "UIScreen.h"
#include "Graphics.h"
#include <shellapi.h>

#define RENDER_IF_IN_BOX( x, y, z, code ) if ( x >= y && x <= ( y + z ) ) code

void UIScreen::Initialize( const Graphics& gfx, ConstantBuffer<Matrices>* mat )
{
	m_cbMatrices = mat;
	m_pDevice = gfx.GetDevice();
	m_pContext = gfx.GetContext();

	m_image.Initialize( m_pDevice.Get(), m_pContext.Get(), *mat );
	m_inputBox.Initialize( m_pDevice.Get(), m_pContext.Get(), *mat );
	m_dropDown.Initialize( m_pDevice.Get(), m_pContext.Get(), *mat );
	m_energyBar.Initialize( m_pDevice.Get(), m_pContext.Get(), *mat );
	m_dataSlider.Initialize( m_pDevice.Get(), m_pContext.Get(), *mat );
	m_colourBlock.Initialize( m_pDevice.Get(), m_pContext.Get(), *mat );
	for ( unsigned int i = 0; i < ARRAYSIZE( m_buttons ); i++ )
		m_buttons[i].Initialize( m_pDevice.Get(), m_pContext.Get(), *mat );
	m_pageSlider.Initialize( m_pDevice.Get(), m_pContext.Get(), *mat );
	m_pageSlider.SetPageSize( m_vScreenSize.y );
}

void UIScreen::Update( const float dt )
{
	if ( m_bMouseLoad )
	{
		m_bMouseLoad = false;
		m_mouseData.LPress = false;
	}

	XMFLOAT2 size = { m_vScreenSize.x * 0.1f, m_vScreenSize.y * 0.1f };
	XMFLOAT2 pos = { m_vScreenSize.x * 0.025f, m_fCurrentY };
	float offset = 0.115f;

	// --- Colour Block Widgets ---
	m_colourBlock.Resolve( { 210, 210, 150 }, pos, size );
	m_colourBlock.Update( dt );
	pos.x += m_vScreenSize.x * offset;

	// --- Image  Widgets ---
	m_image.Resolve( "Resources\\Textures\\UI\\Board\\Board.png", pos, size );
	m_image.Update( dt );
	pos.x += m_vScreenSize.x * offset;

	// --- Data Slider Widgets ---
	m_dataSlider.Resolve( m_iSliderStart, "Resources\\Textures\\UI\\Slider\\Slider Background.png", "Resources\\Textures\\UI\\Slider\\Control Point.png", m_mouseData, pos, size );
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
	if ( m_buttons[0].Resolve( "", Colors::White, m_texturesGithub, m_mouseData, pos, size ) )
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
	m_buttons[1].Resolve( "Example", Colors::White, m_textures, m_mouseData, pos, size );
	m_buttons[1].Update( dt );
	pos.x += m_vScreenSize.x * offset;

	// Quit game
	if ( m_buttons[2].Resolve( "Exit", Colors::White, m_textures, m_mouseData, pos, size ) )
		EventSystem::Instance()->AddEvent( EVENTID::QuitGameEvent );
	m_buttons[2].Update( dt );
	pos.x += m_vScreenSize.x * offset;

	// --- Drop Down Widgets ---
	std::vector<std::string> vValues = { "True", "False" };
	static std::string sValue = vValues[0];
	m_dropDown.Resolve( vValues, pos, size, m_texturesDD, m_texturesDDButton, Colors::White, sValue, m_mouseData );
	if ( m_dropDown.GetSelected() == "False" )
		sValue = "False";
	else
		sValue = "True";
	m_dropDown.Update( dt );
	pos = { m_vScreenSize.x * 0.025f, m_fCurrentY + ( m_vScreenSize.y * 0.1f ) };

	// --- Input Box Widgets ---
	m_inputBox.Resolve( m_sKeys, Colors::White, m_textures, m_mouseData, pos, size );
	m_inputBox.Update( dt );

	// --- Page Slider Widget ---
	m_pageSlider.Resolve(
		Colour( 10.0f, 10.0f, 10.0f ), Colour( 60.0f, 60.0f, 60.0f ), m_mouseData,
		{ m_vScreenSize.x - 30.0f , m_vScreenSize.y * 0.2f },
		{ 30.0f, m_vScreenSize.y * 0.6f } );
	m_pageSlider.Update( dt );
	m_fCurrentY = ( m_vScreenSize.y * 0.2f ) - m_pageSlider.GetPagePos();
	if ( m_fCurrentPY != m_pageSlider.GetPY() )
	{
		m_fCurrentPY = m_pageSlider.GetPY();
		m_bLoadFlag = true;
	}

	// Update render box
	m_fBoxPos = { 0.0f, m_vScreenSize.y * 0.1f };
	m_fBoxSize = { m_vScreenSize.x, m_vScreenSize.y * 0.6f };
}

void UIScreen::Draw( VertexShader& vtx, PixelShader& pix, XMMATRIX worldOrtho, TextRenderer* textRenderer )
{
	RENDER_IF_IN_BOX( m_image.GetTransform()->GetPosition().y, m_fBoxPos.y, m_fBoxSize.y, m_image.Draw( m_pDevice.Get(), m_pContext.Get(), worldOrtho ) );
	RENDER_IF_IN_BOX( m_dropDown.GetTransform()->GetPosition().y, m_fBoxPos.y, m_fBoxSize.y, m_dropDown.Draw( m_pDevice.Get(), m_pContext.Get(), worldOrtho, textRenderer, vtx, pix ) );
	Shaders::BindShaders( m_pContext.Get(), vtx, pix );

	RENDER_IF_IN_BOX( m_inputBox.GetTransform()->GetPosition().y, m_fBoxPos.y, m_fBoxSize.y, m_inputBox.Draw( m_pDevice.Get(), m_pContext.Get(), worldOrtho, textRenderer ) );
	Shaders::BindShaders( m_pContext.Get(), vtx, pix );

	RENDER_IF_IN_BOX( m_energyBar.GetTransform()->GetPosition().y, m_fBoxPos.y, m_fBoxSize.y, m_energyBar.Draw( m_pDevice.Get(), m_pContext.Get(), worldOrtho ) );
	RENDER_IF_IN_BOX( m_dataSlider.GetTransform()->GetPosition().y, m_fBoxPos.y, m_fBoxSize.y, m_dataSlider.Draw( m_pDevice.Get(), m_pContext.Get(), worldOrtho ) );
	RENDER_IF_IN_BOX( m_colourBlock.GetTransform()->GetPosition().y, m_fBoxPos.y, m_fBoxSize.y, m_colourBlock.Draw( m_pDevice.Get(), m_pContext.Get(), worldOrtho ) );
	for ( unsigned int i = 0; i < ARRAYSIZE( m_buttons ); i++ )
	{
		RENDER_IF_IN_BOX( m_buttons[i].GetTransform()->GetPosition().y, m_fBoxPos.y, m_fBoxSize.y, m_buttons[i].Draw( m_pDevice.Get(), m_pContext.Get(), worldOrtho, textRenderer ) );
		Shaders::BindShaders( m_pContext.Get(), vtx, pix );
	}

	m_pageSlider.Draw( m_pDevice.Get(), m_pContext.Get(), worldOrtho );
}

void UIScreen::AddToEvent() noexcept
{
	EventSystem::Instance()->AddClient( EVENTID::KeyInput, this );
	EventSystem::Instance()->AddClient( EVENTID::MousePosition, this );
	EventSystem::Instance()->AddClient( EVENTID::LeftMouseClick, this );
	EventSystem::Instance()->AddClient( EVENTID::LeftMouseRelease, this );
	EventSystem::Instance()->AddClient( EVENTID::RightMouseClick, this );
	EventSystem::Instance()->AddClient( EVENTID::RightMouseRelease, this );
	EventSystem::Instance()->AddClient( EVENTID::MiddleMouseClick, this );
	EventSystem::Instance()->AddClient( EVENTID::MiddleMouseRelease, this );
	EventSystem::Instance()->AddClient( EVENTID::PlayerHealth, this );
	EventSystem::Instance()->AddClient( EVENTID::WindowSizeChangeEvent, this );
}

void UIScreen::RemoveFromEvent() noexcept
{
	EventSystem::Instance()->RemoveClient( EVENTID::KeyInput, this );
	EventSystem::Instance()->RemoveClient( EVENTID::MousePosition, this );
	EventSystem::Instance()->RemoveClient( EVENTID::LeftMouseClick, this );
	EventSystem::Instance()->RemoveClient( EVENTID::LeftMouseRelease, this );
	EventSystem::Instance()->RemoveClient( EVENTID::RightMouseClick, this );
	EventSystem::Instance()->RemoveClient( EVENTID::RightMouseRelease, this );
	EventSystem::Instance()->RemoveClient( EVENTID::MiddleMouseClick, this );
	EventSystem::Instance()->RemoveClient( EVENTID::MiddleMouseRelease, this );
	EventSystem::Instance()->RemoveClient( EVENTID::PlayerHealth, this );
	EventSystem::Instance()->RemoveClient( EVENTID::WindowSizeChangeEvent, this );
}

void UIScreen::HandleEvent( Event* event )
{
	switch ( event->GetEventID() )
	{
	case EVENTID::KeyInput:	{ m_sKeys = *(std::string*)event->GetData(); } break;
	case EVENTID::MousePosition:{ m_mouseData.Pos = *(XMFLOAT2*)event->GetData(); } break;
	case EVENTID::LeftMouseClick:{ m_mouseData.LPress = true; } break;
	case EVENTID::LeftMouseRelease:{ m_mouseData.LPress = false; } break;
	case EVENTID::RightMouseClick:{ m_mouseData.RPress = true; } break;
	case EVENTID::RightMouseRelease:{ m_mouseData.RPress = false; } break;
	case EVENTID::MiddleMouseClick: { m_mouseData.MPress = true; } break;
	case EVENTID::MiddleMouseRelease: { m_mouseData.MPress = false; } break;
	case EVENTID::PlayerHealth: { m_fPlayerHealth = *static_cast<float*>( event->GetData() ); } break;
	case EVENTID::WindowSizeChangeEvent:
	{
		m_vScreenSize = *static_cast<XMFLOAT2*>( event->GetData() );
		m_pageSlider.SetPageSize( m_vScreenSize.y );
	}
	break;
	}
}