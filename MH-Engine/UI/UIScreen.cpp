#include "stdafx.h"
#include "UIScreen.h"
#include "Graphics.h"
#include <shellapi.h>

#define RENDER_IF_IN_BOX( x, y, z, code ) if ( x >= y && x <= ( y + z ) ) code

void UIScreen::Initialize( const Graphics& gfx, ConstantBuffer<Matrices>* mat, const std::vector<Widget>& widgets )
{
	m_cbMatrices = mat;
	m_vWidgets = widgets;
	m_pDevice = gfx.GetDevice();
	m_pContext = gfx.GetContext();
	UpdateWidgets();
}

void UIScreen::UpdateWidgets()
{
	int inputIndex = 0;
	for ( unsigned int i = 0; i < m_vWidgets.size(); i++ )
	{
		if ( m_vWidgets[i].GetType() == "Button" )
		{
			Button_Widget button;
			button.Initialize( m_pDevice.Get(), m_pContext.Get(), *m_cbMatrices );
			button.IntializeWidget( m_vWidgets[i] );
			m_vButtons.push_back( std::move( button ) );
		}
		else if ( m_vWidgets[i].GetType() == "Colour Block" )
		{
			ColourBlock_Widget colourBlock;
			colourBlock.Initialize( m_pDevice.Get(), m_pContext.Get(), *m_cbMatrices );
			colourBlock.IntializeWidget( m_vWidgets[i] );
			m_vColourBlocks.push_back( std::move( colourBlock ) );
		}
		else if ( m_vWidgets[i].GetType() == "Data Slider" )
		{
			DataSlider_Widget dataSlider;
			dataSlider.Initialize( m_pDevice.Get(), m_pContext.Get(), *m_cbMatrices );
			dataSlider.IntializeWidget( m_vWidgets[i] );
			m_vDataSliders.push_back( std::move( dataSlider ) );
		}
		else if ( m_vWidgets[i].GetType() == "Drop Down" )
		{
			DropDown_Widget dropDown;
			dropDown.Initialize( m_pDevice.Get(), m_pContext.Get(), *m_cbMatrices );
			dropDown.IntializeWidget( m_vWidgets[i] );
			m_vDropDowns.push_back( std::move( dropDown ) );
		}
		else if ( m_vWidgets[i].GetType() == "Energy Bar" )
		{
			EnergyBar_Widget energyBar;
			energyBar.Initialize( m_pDevice.Get(), m_pContext.Get(), *m_cbMatrices );
			energyBar.IntializeWidget( m_vWidgets[i] );
			m_vEnergyBars.push_back( std::move( energyBar ) );
		}
		else if ( m_vWidgets[i].GetType() == "Image" )
		{
			Image_Widget image;
			image.Initialize( m_pDevice.Get(), m_pContext.Get(), *m_cbMatrices );
			image.IntializeWidget( m_vWidgets[i] );
			m_vImages.push_back( std::move( image ) );
		}
		else if ( m_vWidgets[i].GetType() == "Input" )
		{
			Input_Widget input;
			input.Initialize( m_pDevice.Get(), m_pContext.Get(), *m_cbMatrices, inputIndex );
			input.IntializeWidget( m_vWidgets[i] );
			m_vInputs.push_back( std::move( input ) );
			inputIndex++;
		}
		else if ( m_vWidgets[i].GetType() == "Page Slider" )
		{
			PageSlider_Widget pageSlider;
			pageSlider.Initialize( m_pDevice.Get(), m_pContext.Get(), *m_cbMatrices );
			pageSlider.IntializeWidget( m_vWidgets[i] );
			pageSlider.SetPageSize( m_vScreenSize.y );
			m_vPageSliders.push_back( std::move( pageSlider ) );
		}
	}
}

void UIScreen::Update( const float dt, const std::vector<Widget>& widgets )
{
	m_vWidgets = widgets;
	m_vButtons.clear();
	m_vColourBlocks.clear();
	m_vDataSliders.clear();
	m_vDropDowns.clear();
	m_vEnergyBars.clear();
	m_vImages.clear();

	// Save input widget data before clearing
	std::vector<bool> inputSelections;
	std::vector<std::string> inputStrings;
	for ( unsigned int i = 0; i < m_vInputs.size(); i++ )
	{
		inputSelections.push_back( m_vInputs[i].GetSelected() );
		inputStrings.push_back( m_vInputs[i].GetCurrentText() );
	}
	m_vInputs.clear();

	m_vPageSliders.clear();
	UpdateWidgets();

	// Re-add input widget data
	for ( unsigned int i = 0; i < m_vInputs.size(); i++ )
	{
		if ( inputSelections.size() == i ) inputSelections.push_back( false );
		if ( inputStrings.size() == i ) inputStrings.push_back( "" );
		m_vInputs[i].SetCurrentText( inputStrings[i] );
		m_vInputs[i].SetSelected( inputSelections[i] );
	}

	if ( !m_mouseData.LPress )
		m_mouseData.Locked = false;

	for ( unsigned int i = 0; i < m_vButtons.size(); i++ )
	{
		if ( m_vButtons[i].GetAction() == "Link" )
		{
			// GitHub Link
			if ( m_vButtons[i].Resolve( "", Colors::White, m_texturesGithub, m_mouseData ) )
				if ( !m_bOpenLink && m_bOpen )
					m_bOpenLink = true;
			m_vButtons[i].Update( dt );
			if ( !m_vButtons[i].GetIsPressed() )
				m_bOpen = true;
			if ( m_bOpenLink )
			{
				ShellExecute( 0, 0, L"https://github.com/Nine-Byte-Warriors", 0, 0, SW_SHOW );
				m_bOpenLink = false;
				m_bOpen = false;
			}
		}
		else if ( m_vButtons[i].GetAction() == "Close" )
		{
			// Quit Game
			if ( m_vButtons[i].Resolve( "Quit Game", Colors::White, m_textures, m_mouseData ) )
				EventSystem::Instance()->AddEvent( EVENTID::QuitGameEvent );
			m_vButtons[i].Update( dt );
		}
		else
		{
			// Default
			m_vButtons[i].Resolve( "-PlaceHolder-", Colors::White, m_textures, m_mouseData );
			m_vButtons[i].Update( dt );
		}
	}

	for ( unsigned int i = 0; i < m_vColourBlocks.size(); i++ )
	{
		// Doesn't need actions
		m_vColourBlocks[i].Resolve( { 210, 210, 150 } );
		m_vColourBlocks[i].Update( dt );
	}

	for ( unsigned int i = 0; i < m_vDataSliders.size(); i++ )
	{
		if ( m_vDataSliders[i].GetAction() == "Master Volume" )
		{
			// Create a slider that syncs with master volume
		}
		else  if ( m_vDataSliders[i].GetAction() == "Music Volume" )
		{
			// Create a slider that syncs with musics volume
		}
		else
		{
			// Default
			m_vDataSliders[i].Resolve( m_iSliderStart, "Resources\\Textures\\UI\\Slider\\Slider Background.png",
				"Resources\\Textures\\UI\\Slider\\Control Point.png", m_mouseData );
			m_vDataSliders[i].Update( dt );
		}
	}

	for ( unsigned int i = 0; i < m_vDropDowns.size(); i++ )
	{
		if ( m_vDropDowns[i].GetAction() == "Resolution" )
		{
			// Create a drop down that allows user to change resolution
		}
		else
		{
			// Default
			std::vector<std::string> vValues = { "True", "False" };
			static std::string sValue = vValues[0];
			m_vDropDowns[i].Resolve( vValues, m_texturesDD, m_texturesDDButton, Colors::White, sValue, m_mouseData );
			if ( m_vDropDowns[i].GetSelected() == "False" )
				sValue = "False";
			else
				sValue = "True";
			m_vDropDowns[i].Update( dt );
		}
	}

	for ( unsigned int i = 0; i < m_vEnergyBars.size(); i++ )
	{
		if ( m_vEnergyBars[i].GetAction() == "Player Health" )
		{
			// Bar that displays the player's health
		}
		else if ( m_vEnergyBars[i].GetAction() == "Enemy Health" )
		{
			// Bar that displays an enemy's health
		}
		else
		{
			// Default
			std::string temp = m_textures[2];
			m_textures[2] = "";
			m_vEnergyBars[i].Resolve( m_textures, m_fPlayerHealth );
			m_textures[2] = temp;
			m_vEnergyBars[i].Update( dt );
		}
	}

	for ( unsigned int i = 0; i < m_vImages.size(); i++ )
	{
		m_vImages[i].Resolve( "Resources\\Textures\\UI\\Board\\Board.png" );
		m_vImages[i].Update( dt );
	}

	for ( unsigned int i = 0; i < m_vInputs.size(); i++ )
	{
		if ( m_vInputs[i].GetAction() == "Player Name" )
		{
			// Input that allows the user to enter their name
		}
		else
		{
			// Default
			m_vInputs[i].Resolve( m_sKeys, Colors::White, m_textures, m_mouseData, m_iInputIndex );
			m_vInputs[i].Update( dt );
		}
	}

	for ( unsigned int i = 0; i < m_vPageSliders.size(); i++ )
	{
		m_vPageSliders[i].Resolve( Colour( 10.0f, 10.0f, 10.0f ), Colour( 60.0f, 60.0f, 60.0f ), m_mouseData );
		m_vPageSliders[i].SetPosition( { m_vScreenSize.x - 30.0f, m_vScreenSize.y * 0.2f } );
		m_vPageSliders[i].SetSize( { 30.0f, m_vScreenSize.y * 0.6f } );
		m_vPageSliders[i].Update( dt );
		m_fCurrentY = ( m_vScreenSize.y * 0.2f ) - m_vPageSliders[i].GetPagePos();
		if ( m_fCurrentPY != m_vPageSliders[i].GetPY() )
		{
			m_fCurrentPY = m_vPageSliders[i].GetPY();
			m_bLoadFlag = true;
		}
		if ( m_bUpdateSlider )
			m_vPageSliders[i].SetPageSize( m_vScreenSize.y );
	}

	// Update render box
	if ( m_vPageSliders.size() > 0 )
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
	unsigned int widgetAmount =
		m_vButtons.size() + m_vColourBlocks.size() +
		m_vDataSliders.size() + m_vDropDowns.size() + m_vInputs.size() +
		m_vEnergyBars.size() + m_vImages.size() + m_vPageSliders.size();

	for ( unsigned int i = 0; i < widgetAmount; i++ )
	{
		for ( unsigned int j = 0; j < m_vButtons.size(); j++ )
		{
			if ( m_vButtons[j].GetZIndex() == i )
			{
				if ( !m_vButtons[j].GetIsHidden() )
				{
					RENDER_IF_IN_BOX( m_vButtons[j].GetTransform()->GetPosition().y, m_fBoxPos.y, m_fBoxSize.y,
						m_vButtons[j].Draw( m_pDevice.Get(), m_pContext.Get(), worldOrtho, textRenderer ) );
					Shaders::BindShaders( m_pContext.Get(), vtx, pix );
				}
				break;
			}
		}

		for ( unsigned int j = 0; j < m_vColourBlocks.size(); j++ )
		{
			if ( m_vColourBlocks[j].GetZIndex() == i )
			{
				if ( !m_vColourBlocks[j].GetIsHidden() )
				{
					RENDER_IF_IN_BOX( m_vColourBlocks[j].GetTransform()->GetPosition().y, m_fBoxPos.y, m_fBoxSize.y,
						m_vColourBlocks[j].Draw( m_pDevice.Get(), m_pContext.Get(), worldOrtho ) );
				}
				break;
			}
		}

		for ( unsigned int j = 0; j < m_vDataSliders.size(); j++ )
		{
			if ( m_vDataSliders[j].GetZIndex() == i )
			{
				if ( !m_vDataSliders[j].GetIsHidden() )
				{
					RENDER_IF_IN_BOX( m_vDataSliders[j].GetTransform()->GetPosition().y, m_fBoxPos.y, m_fBoxSize.y,
						m_vDataSliders[j].Draw( m_pDevice.Get(), m_pContext.Get(), worldOrtho ) );
				}
				break;
			}
		}

		for ( unsigned int j = 0; j < m_vDropDowns.size(); j++ )
		{
			if ( m_vDropDowns[j].GetZIndex() == i )
			{
				if ( !m_vDropDowns[j].GetIsHidden() )
				{
					RENDER_IF_IN_BOX( m_vDropDowns[j].GetTransform()->GetPosition().y, m_fBoxPos.y, m_fBoxSize.y,
						m_vDropDowns[j].Draw( m_pDevice.Get(), m_pContext.Get(), worldOrtho, textRenderer, vtx, pix ) );
					Shaders::BindShaders( m_pContext.Get(), vtx, pix );
				}
				break;
			}
		}

		for ( unsigned int j = 0; j < m_vEnergyBars.size(); j++ )
		{
			if ( m_vEnergyBars[j].GetZIndex() == i )
			{
				if ( !m_vEnergyBars[j].GetIsHidden() )
				{
					RENDER_IF_IN_BOX( m_vEnergyBars[j].GetTransform()->GetPosition().y, m_fBoxPos.y, m_fBoxSize.y,
						m_vEnergyBars[j].Draw( m_pDevice.Get(), m_pContext.Get(), worldOrtho ) );
				}
				break;
			}
		}

		for ( unsigned int j = 0; j < m_vImages.size(); j++ )
		{
			if ( m_vImages[j].GetZIndex() == i )
			{
				if ( !m_vImages[j].GetIsHidden() )
				{
					RENDER_IF_IN_BOX( m_vImages[j].GetTransform()->GetPosition().y, m_fBoxPos.y, m_fBoxSize.y,
						m_vImages[j].Draw( m_pDevice.Get(), m_pContext.Get(), worldOrtho ) );
				}
				break;
			}
		}

		for ( unsigned int j = 0; j < m_vInputs.size(); j++ )
		{
			if ( m_vInputs[j].GetZIndex() == i )
			{
				if ( !m_vInputs[j].GetIsHidden() )
				{
					RENDER_IF_IN_BOX( m_vInputs[j].GetTransform()->GetPosition().y, m_fBoxPos.y, m_fBoxSize.y,
						m_vInputs[j].Draw( m_pDevice.Get(), m_pContext.Get(), worldOrtho, textRenderer ) );
					Shaders::BindShaders( m_pContext.Get(), vtx, pix );
				}
				break;
			}
		}

		for ( unsigned int j = 0; j < m_vPageSliders.size(); j++ )
		{
			if ( m_vPageSliders[j].GetZIndex() == i )
			{
				if ( !m_vPageSliders[j].GetIsHidden() )
				{
					RENDER_IF_IN_BOX( m_vPageSliders[j].GetPosition().y, m_fBoxPos.y, m_fBoxSize.y,
						m_vPageSliders[j].Draw( m_pDevice.Get(), m_pContext.Get(), worldOrtho ) );
				}
				break;
			}
		}
	}
}

void UIScreen::AddToEvent() noexcept
{
	EventSystem::Instance()->AddClient( EVENTID::KeyInput, this );
#if _DEBUG
	EventSystem::Instance()->AddClient( EVENTID::ImGuiMousePosition, this );
#else
	EventSystem::Instance()->AddClient( EVENTID::MousePosition, this );
#endif
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
#if _DEBUG
	EventSystem::Instance()->RemoveClient( EVENTID::ImGuiMousePosition, this );
#else
	EventSystem::Instance()->RemoveClient( EVENTID::MousePosition, this );
#endif
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
	case EVENTID::LeftMouseClick:{ m_mouseData.LPress = true; } break;
	case EVENTID::LeftMouseRelease:{ m_mouseData.LPress = false; } break;
	case EVENTID::RightMouseClick:{ m_mouseData.RPress = true; } break;
	case EVENTID::RightMouseRelease:{ m_mouseData.RPress = false; } break;
	case EVENTID::MiddleMouseClick: { m_mouseData.MPress = true; } break;
	case EVENTID::MiddleMouseRelease: { m_mouseData.MPress = false; } break;
	case EVENTID::PlayerHealth: { m_fPlayerHealth = *static_cast<float*>( event->GetData() ); } break;
#if _DEBUG
	case EVENTID::ImGuiMousePosition:
	{
		Vector2f mousePos = *(Vector2f*)event->GetData();
		m_mouseData.Pos = XMFLOAT2( mousePos.x, mousePos.y );
	}
	break;
#else
	case EVENTID::MousePosition:
	{
		m_mouseData.Pos = *(XMFLOAT2*)event->GetData();
	}
	break;
#endif
	case EVENTID::WindowSizeChangeEvent:
	{
		m_vScreenSize = *static_cast<XMFLOAT2*>( event->GetData() );
		m_bUpdateSlider = true;
	}
	break;
	}
}