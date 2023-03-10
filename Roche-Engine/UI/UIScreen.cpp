#include "stdafx.h"
#include "UIScreen.h"
#include "Graphics.h"
#include <shellapi.h>

#if _DEBUG
extern bool g_bDebug;
#endif
#define RENDER_IF_IN_BOX( x, y, z, code ) if ( x >= y && x <= ( y + z ) ) code

void UIScreen::Initialize( const Graphics& gfx, ConstantBuffer<Matrices>* mat, const std::vector<std::shared_ptr<Widget>>& widgets )
{
	m_cbMatrices = mat;
	m_vWidgets = widgets;
	m_pDevice = gfx.GetDevice();
	m_pContext = gfx.GetContext();
	InitializeWidgets();
}

void UIScreen::InitializeWidgets()
{
	for ( unsigned int i = 0; i < m_vWidgets.size(); i++ )
	{
		m_vWidgets[i]->GetButtonWidget()->Initialize( m_pDevice.Get(), m_pContext.Get(), *m_cbMatrices );
		m_vWidgets[i]->GetColourBlockWidget()->Initialize( m_pDevice.Get(), m_pContext.Get(), *m_cbMatrices );
		m_vWidgets[i]->GetDataSliderWidget()->Initialize( m_pDevice.Get(), m_pContext.Get(), *m_cbMatrices );
		m_vWidgets[i]->GetDropDownWidget()->Initialize( m_pDevice.Get(), m_pContext.Get(), *m_cbMatrices );
		m_vWidgets[i]->GetEnergyBarWidget()->Initialize( m_pDevice.Get(), m_pContext.Get(), *m_cbMatrices );
		m_vWidgets[i]->GetImageWidget()->Initialize( m_pDevice.Get(), m_pContext.Get(), *m_cbMatrices );
		m_vWidgets[i]->GetInputWidget()->Initialize( m_pDevice.Get(), m_pContext.Get(), *m_cbMatrices, i );
		m_vWidgets[i]->GetPageSliderWidget()->Initialize( m_pDevice.Get(), m_pContext.Get(), *m_cbMatrices, m_vScreenSize.x, m_vScreenSize.y );
	}
}

void UIScreen::Update( const float dt )
{
	if ( !m_mouseData.LPress )
		m_mouseData.Locked = false;

	// first, determine if any of the widgets is a page slider
	m_bUsingPageSlider = false;
	for ( unsigned int i = 0; i < m_vWidgets.size(); i++ )
	{
		if ( m_vWidgets[i]->GetType() == "Page Slider" )
		{
			m_bUsingPageSlider = true;
			break;
		}
	}

	for ( unsigned int i = 0; i < m_vWidgets.size(); i++ )
	{
		if ( m_bUsingPageSlider && m_vWidgets[i]->GetType() != "Page Slider" )
			m_vWidgets[i]->SetPosition( { m_vWidgets[i]->GetPosition().x, m_vWidgets[i]->GetPosition().y + ( ( m_fCurrentY / m_vScreenSize.y ) * 100.0f ) } );

#pragma region BUTTONS
		if ( m_vWidgets[i]->GetType() == "Button" )
		{
			if ( m_vWidgets[i]->GetAction() == "Link" )
			{
				// GitHub Link
				if ( m_vWidgets[i]->GetButtonWidget()->Resolve( "", Colors::White, m_texturesGithub, m_mouseData ) )
					if ( !m_bOpenLink && m_bOpen )
						m_bOpenLink = true;

				if ( !m_vWidgets[i]->GetButtonWidget()->GetIsPressed() )
					m_bOpen = true;

				if ( m_bOpenLink )
				{
					ShellExecute( 0, 0, L"https://github.com/Nine-Byte-Warriors", 0, 0, SW_SHOW );
					m_bOpenLink = false;
					m_bOpen = false;
				}
			}
			if ( m_vWidgets[i]->GetAction() == "Close" )
			{
				// Quit Game
				if ( m_vWidgets[i]->GetButtonWidget()->Resolve( "Quit Game", Colors::White, m_textures, m_mouseData ) )
					EventSystem::Instance()->AddEvent( EVENTID::QuitGameEvent );
			}
			if ( m_vWidgets[i]->GetAction() == "" )
			{
				m_vWidgets[i]->GetButtonWidget()->Resolve( "", Colors::White, m_textures, m_mouseData );
			}
			m_vWidgets[i]->GetButtonWidget()->Update( dt );
		}
#pragma endregion
#pragma region COLOUR_BLOCKS
		else if ( m_vWidgets[i]->GetType() == "Colour Block" )
		{
			// Doesn't need actions
			m_vWidgets[i]->GetColourBlockWidget()->Resolve( { 210, 210, 150 } );
			m_vWidgets[i]->GetColourBlockWidget()->Update( dt );
		}
#pragma endregion
#pragma region DATA_SLIDERS
		else if ( m_vWidgets[i]->GetType() == "Data Slider" )
		{
			if ( m_vWidgets[i]->GetAction() == "Master Volume" )
			{
				// Create a slider that syncs with master volume
			}
			if ( m_vWidgets[i]->GetAction() == "Music Volume" )
			{
				// Create a slider that syncs with musics volume
			}
			if ( m_vWidgets[i]->GetAction() == "" )
			{
				m_vWidgets[i]->GetDataSliderWidget()->Resolve( m_iSliderStart,
					"Resources\\Textures\\UI\\Slider\\Slider Background.png",
					"Resources\\Textures\\UI\\Slider\\Control Point.png", m_mouseData );
			}
			m_vWidgets[i]->GetDataSliderWidget()->Update( dt );
		}
#pragma endregion
#pragma region DROP_DOWNS
		else if ( m_vWidgets[i]->GetType() == "Drop Down" )
		{
			if ( m_vWidgets[i]->GetAction() == "Resolution" )
			{
				// Create a drop down that allows user to change resolution
			}
			if ( m_vWidgets[i]->GetAction() == "" )
			{
				std::vector<std::string> vValues = { "True", "False" };
				static std::string sValue = vValues[0];
				m_vWidgets[i]->GetDropDownWidget()->Resolve( vValues, m_texturesDD, m_texturesDDButton, Colors::White, sValue, m_mouseData );
				if ( m_vWidgets[i]->GetDropDownWidget()->GetSelected() == "False" )
					sValue = "False";
				else
					sValue = "True";
			}
			m_vWidgets[i]->GetDropDownWidget()->Update( dt );
		}
#pragma endregion
#pragma region ENERGY_BARS
		else if ( m_vWidgets[i]->GetType() == "Energy Bar" )
		{
			if ( m_vWidgets[i]->GetAction() == "Player Health" )
			{
				// Bar that displays the player's health
			}
			if ( m_vWidgets[i]->GetAction() == "Enemy Health" )
			{
				// Bar that displays an enemy's health
			}
			if ( m_vWidgets[i]->GetAction() == "" )
			{
				static float health = 100.0f;
				std::string temp = m_textures[2];
				m_textures[2] = "";
				m_vWidgets[i]->GetEnergyBarWidget()->Resolve( m_textures, health );
				m_textures[2] = temp;
			}
			m_vWidgets[i]->GetEnergyBarWidget()->Update( dt );
		}
#pragma endregion
#pragma region IMAGES
		else if ( m_vWidgets[i]->GetType() == "Image" )
		{
			m_vWidgets[i]->GetImageWidget()->Resolve( "Resources\\Textures\\UI\\Board\\Board.png" );
			m_vWidgets[i]->GetImageWidget()->Update( dt );
		}
#pragma endregion
#pragma region INPUT
		else if ( m_vWidgets[i]->GetType() == "Input" )
		{

			if ( m_vWidgets[i]->GetAction() == "Player Name" )
			{
				// Input that allows the user to enter their name
			}
			if ( m_vWidgets[i]->GetAction() == "" )
			{
				m_vWidgets[i]->GetInputWidget()->Resolve( m_sKeys, Colors::White, m_textures, m_mouseData, i );
			}
			m_vWidgets[i]->GetInputWidget()->Update( dt );
		}
#pragma endregion

#pragma region PAGE_SLIDERS
		else if ( m_vWidgets[i]->GetType() == "Page Slider" )
		{
			m_vWidgets[i]->SetPosition( { m_vScreenSize.x - 30.0f, m_vScreenSize.y * 0.2f } );
			m_vWidgets[i]->SetSize( { 30.0f, m_vScreenSize.y * 0.6f } );

			m_vWidgets[i]->GetPageSliderWidget()->Resolve( Colour( 10.0f, 10.0f, 10.0f ), Colour( 60.0f, 60.0f, 60.0f ), m_mouseData );
			m_vWidgets[i]->GetPageSliderWidget()->Update( dt );

			m_fCurrentY = ( m_vScreenSize.y * 0.2f ) - m_vWidgets[i]->GetPageSliderWidget()->GetPagePos();

			if ( m_bUpdatePageSlider )
			{
				m_vWidgets[i]->GetPageSliderWidget()->SetPageSizeY( m_vScreenSize.y );
				m_bUpdatePageSlider = false;
			}
		}
#pragma endregion
	}

	// Update render box
	if ( m_bUsingPageSlider )
	{
		m_fBoxPos = { 0.0f, m_vScreenSize.y * 0.1f };
		m_fBoxSize = { m_vScreenSize.x, m_vScreenSize.y * 0.6f };
	}
	else
	{
		m_fBoxPos = { 0.0f, 0.0f };
		m_fBoxSize = { m_vScreenSize.x, m_vScreenSize.y };
	}
}

void UIScreen::Draw( VertexShader& vtx, PixelShader& pix, XMMATRIX worldOrtho, TextRenderer* textRenderer )
{
	for ( unsigned int i = 0; i < m_vWidgets.size(); i++ )
	{
		for ( unsigned int j = 0; j < m_vWidgets.size(); j++ )
		{
			if ( m_vWidgets[j]->GetZIndex() == i )
			{
				if ( !m_vWidgets[j]->GetIsHidden() )
				{
					Shaders::BindShaders( m_pContext.Get(), vtx, pix );
					if ( m_vWidgets[j]->GetType() == "Button" )
					{
						RENDER_IF_IN_BOX( m_vWidgets[j]->GetButtonWidget()->GetTransform()->GetPosition().y, m_fBoxPos.y, m_fBoxSize.y,
							m_vWidgets[j]->GetButtonWidget()->Draw( m_pDevice.Get(), m_pContext.Get(), worldOrtho, textRenderer ) );
						Shaders::BindShaders( m_pContext.Get(), vtx, pix );
						break;
					}
					else if ( m_vWidgets[j]->GetType() == "Colour Block" )
					{
						RENDER_IF_IN_BOX( m_vWidgets[j]->GetColourBlockWidget()->GetTransform()->GetPosition().y, m_fBoxPos.y, m_fBoxSize.y,
							m_vWidgets[j]->GetColourBlockWidget()->Draw( m_pDevice.Get(), m_pContext.Get(), worldOrtho ) );
						Shaders::BindShaders( m_pContext.Get(), vtx, pix );
						break;
					}
					else if ( m_vWidgets[j]->GetType() == "Data Slider" )
					{
						RENDER_IF_IN_BOX( m_vWidgets[j]->GetDataSliderWidget()->GetTransform()->GetPosition().y, m_fBoxPos.y, m_fBoxSize.y,
							m_vWidgets[j]->GetDataSliderWidget()->Draw( m_pDevice.Get(), m_pContext.Get(), worldOrtho ) );
						Shaders::BindShaders( m_pContext.Get(), vtx, pix );
						break;
					}
					else if ( m_vWidgets[j]->GetType() == "Drop Down" )
					{
						RENDER_IF_IN_BOX( m_vWidgets[j]->GetDropDownWidget()->GetTransform()->GetPosition().y, m_fBoxPos.y, m_fBoxSize.y,
							m_vWidgets[j]->GetDropDownWidget()->Draw( m_pDevice.Get(), m_pContext.Get(), worldOrtho, textRenderer, vtx, pix ) );
						Shaders::BindShaders( m_pContext.Get(), vtx, pix );
						break;
					}
					else if ( m_vWidgets[j]->GetType() == "Energy Bar" )
					{
						RENDER_IF_IN_BOX( m_vWidgets[j]->GetEnergyBarWidget()->GetTransform()->GetPosition().y, m_fBoxPos.y, m_fBoxSize.y,
							m_vWidgets[j]->GetEnergyBarWidget()->Draw( m_pDevice.Get(), m_pContext.Get(), worldOrtho ) );
						Shaders::BindShaders( m_pContext.Get(), vtx, pix );
						break;
					}
					else if ( m_vWidgets[j]->GetType() == "Image" )
					{
						RENDER_IF_IN_BOX( m_vWidgets[j]->GetImageWidget()->GetTransform()->GetPosition().y, m_fBoxPos.y, m_fBoxSize.y,
							m_vWidgets[j]->GetImageWidget()->Draw( m_pDevice.Get(), m_pContext.Get(), worldOrtho ) );
						Shaders::BindShaders( m_pContext.Get(), vtx, pix );
						break;
					}
					else if ( m_vWidgets[j]->GetType() == "Input" )
					{
						RENDER_IF_IN_BOX( m_vWidgets[j]->GetInputWidget()->GetTransform()->GetPosition().y, m_fBoxPos.y, m_fBoxSize.y,
							m_vWidgets[j]->GetInputWidget()->Draw( m_pDevice.Get(), m_pContext.Get(), worldOrtho, textRenderer ) );
						Shaders::BindShaders( m_pContext.Get(), vtx, pix );
						break;
					}
					else if ( m_vWidgets[j]->GetType() == "Page Slider" )
					{
						RENDER_IF_IN_BOX( m_vWidgets[j]->GetPageSliderWidget()->GetTransform()->GetPosition().y, m_fBoxPos.y, m_fBoxSize.y,
							m_vWidgets[j]->GetPageSliderWidget()->Draw( m_pDevice.Get(), m_pContext.Get(), worldOrtho ) );
						Shaders::BindShaders( m_pContext.Get(), vtx, pix );
						break;
					}
				}
			}
		}
	}
}

void UIScreen::AddToEvent() noexcept
{
	EventSystem::Instance()->AddClient( EVENTID::CharInput, this );
#if _DEBUG
	EventSystem::Instance()->AddClient( EVENTID::ImGuiMousePosition, this );
#endif
	EventSystem::Instance()->AddClient( EVENTID::MousePosition, this );
	EventSystem::Instance()->AddClient( EVENTID::LeftMouseClick, this );
	EventSystem::Instance()->AddClient( EVENTID::LeftMouseRelease, this );
	EventSystem::Instance()->AddClient( EVENTID::RightMouseClick, this );
	EventSystem::Instance()->AddClient( EVENTID::RightMouseRelease, this );
	EventSystem::Instance()->AddClient( EVENTID::MiddleMouseClick, this );
	EventSystem::Instance()->AddClient( EVENTID::MiddleMouseRelease, this );
	EventSystem::Instance()->AddClient( EVENTID::WindowSizeChangeEvent, this );
}

void UIScreen::RemoveFromEvent() noexcept
{
	EventSystem::Instance()->RemoveClient( EVENTID::CharInput, this );
#if _DEBUG
	EventSystem::Instance()->RemoveClient( EVENTID::ImGuiMousePosition, this );
#endif
	EventSystem::Instance()->RemoveClient( EVENTID::MousePosition, this );
	EventSystem::Instance()->RemoveClient( EVENTID::LeftMouseClick, this );
	EventSystem::Instance()->RemoveClient( EVENTID::LeftMouseRelease, this );
	EventSystem::Instance()->RemoveClient( EVENTID::RightMouseClick, this );
	EventSystem::Instance()->RemoveClient( EVENTID::RightMouseRelease, this );
	EventSystem::Instance()->RemoveClient( EVENTID::MiddleMouseClick, this );
	EventSystem::Instance()->RemoveClient( EVENTID::MiddleMouseRelease, this );
	EventSystem::Instance()->RemoveClient( EVENTID::WindowSizeChangeEvent, this );
}

void UIScreen::HandleEvent( Event* event )
{
	switch ( event->GetEventID() )
	{
	case EVENTID::CharInput: { m_sKeys = *(std::string*)event->GetData(); } break;
	case EVENTID::LeftMouseClick:{ m_mouseData.LPress = true; } break;
	case EVENTID::LeftMouseRelease:{ m_mouseData.LPress = false; } break;
	case EVENTID::RightMouseClick:{ m_mouseData.RPress = true; } break;
	case EVENTID::RightMouseRelease:{ m_mouseData.RPress = false; } break;
	case EVENTID::MiddleMouseClick: { m_mouseData.MPress = true; } break;
	case EVENTID::MiddleMouseRelease: { m_mouseData.MPress = false; } break;
#if _DEBUG
	case EVENTID::ImGuiMousePosition:
	{
		if ( !g_bDebug ) return;
		Vector2f mousePos = *(Vector2f*)event->GetData();
		m_mouseData.Pos = XMFLOAT2( mousePos.x, mousePos.y );
	}
	break;
#endif
	case EVENTID::MousePosition:
	{
#if _DEBUG
		if ( g_bDebug ) return;
#endif
		m_mouseData.Pos = *(XMFLOAT2*)event->GetData();
	}
	break;
	case EVENTID::WindowSizeChangeEvent:
	{
		m_vScreenSize = *static_cast<XMFLOAT2*>( event->GetData() );
		m_bUpdatePageSlider = true;
	}
	break;
	}
}