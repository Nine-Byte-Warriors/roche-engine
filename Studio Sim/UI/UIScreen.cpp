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

	for ( unsigned int i = 0; i < m_vWidgets.size(); i++ )
	{
		if ( Button_Widget* widget = dynamic_cast<Button_Widget*>( &m_vWidgets[i] ) )
			widget->Initialize( m_pDevice.Get(), m_pContext.Get(), *mat );
		else if ( ColourBlock_Widget* widget = dynamic_cast<ColourBlock_Widget*>( &m_vWidgets[i] ) )
			widget->Initialize( m_pDevice.Get(), m_pContext.Get(), *mat );
		else if ( DataSlider_Widget* widget = dynamic_cast<DataSlider_Widget*>( &m_vWidgets[i] ) )
			widget->Initialize( m_pDevice.Get(), m_pContext.Get(), *mat );
		else if ( DropDown_Widget* widget = dynamic_cast<DropDown_Widget*>( &m_vWidgets[i] ) )
			widget->Initialize( m_pDevice.Get(), m_pContext.Get(), *mat );
		else if ( EnergyBar_Widget* widget = dynamic_cast<EnergyBar_Widget*>( &m_vWidgets[i] ) )
			widget->Initialize( m_pDevice.Get(), m_pContext.Get(), *mat );
		else if ( Image_Widget* widget = dynamic_cast<Image_Widget*>( &m_vWidgets[i] ) )
			widget->Initialize( m_pDevice.Get(), m_pContext.Get(), *mat );
		else if ( Input_Widget* widget = dynamic_cast<Input_Widget*>( &m_vWidgets[i] ) )
			widget->Initialize( m_pDevice.Get(), m_pContext.Get(), *mat );
		else if ( PageSlider_Widget* widget = dynamic_cast<PageSlider_Widget*>( &m_vWidgets[i] ) )
		{
			widget->Initialize( m_pDevice.Get(), m_pContext.Get(), *mat );
			widget->SetPageSize( m_vScreenSize.y );
		}
	}
}

void UIScreen::Update( const float dt )
{
	if ( !m_mouseData.LPress )
		m_mouseData.Locked = false;

	XMFLOAT2 size = { m_vScreenSize.x * 0.1f, m_vScreenSize.y * 0.1f };
	XMFLOAT2 pos = { m_vScreenSize.x * 0.025f, m_fCurrentY };
	float offset = 0.115f;

	for ( unsigned int i = 0; i < m_vWidgets.size(); i++ )
	{
		if ( Button_Widget* widget = dynamic_cast<Button_Widget*>( &m_vWidgets[i] ) )
		{
			XMFLOAT2 posButton = XMFLOAT2( widget->GetTransform()->GetPosition().x, widget->GetTransform()->GetPosition().y );
			switch ( widget->GetType() )
			{
			case Button_Widget::ButtonType::Quit_Game:
			{
				// Quit Game
				if ( widget->Resolve( "Example", Colors::White, m_texturesGithub, m_mouseData, posButton, size ) )
					EventSystem::Instance()->AddEvent( EVENTID::QuitGameEvent );
				widget->Update( dt );
			}
			break;
			case Button_Widget::ButtonType::GitHub_Link:
			{
				// GitHub Link
				if ( widget->Resolve( "", Colors::White, m_texturesGithub, m_mouseData, posButton, size ) )
					if ( !m_bOpenLink && m_bOpen )
						m_bOpenLink = true;
				widget->Update( dt );
				if ( !widget->GetIsPressed() )
					m_bOpen = true;
				if ( m_bOpenLink )
				{
					ShellExecute( 0, 0, L"https://github.com/Nine-Byte-Warriors", 0, 0, SW_SHOW );
					m_bOpenLink = false;
					m_bOpen = false;
				}
			}
			break;
			}
		}
		else if ( ColourBlock_Widget* widget = dynamic_cast<ColourBlock_Widget*>( &m_vWidgets[i] ) )
		{
			widget->Resolve( { 210, 210, 150 }, pos, size );
			widget->Update( dt );
		}
		else if ( DataSlider_Widget* widget = dynamic_cast<DataSlider_Widget*>( &m_vWidgets[i] ) )
		{
			widget->Resolve( m_iSliderStart, "Resources\\Textures\\UI\\Slider\\Slider Background.png",
				"Resources\\Textures\\UI\\Slider\\Control Point.png", m_mouseData, pos, size );
			widget->Update( dt );
		}
		else if ( DropDown_Widget* widget = dynamic_cast<DropDown_Widget*>( &m_vWidgets[i] ) )
		{
			std::vector<std::string> vValues = { "True", "False" };
			static std::string sValue = vValues[0];
			widget->Resolve( vValues, pos, size, m_texturesDD, m_texturesDDButton, Colors::White, sValue, m_mouseData );
			if ( widget->GetSelected() == "False" )
				sValue = "False";
			else
				sValue = "True";
			widget->Update( dt );
		}
		else if ( EnergyBar_Widget* widget = dynamic_cast<EnergyBar_Widget*>( &m_vWidgets[i] ) )
		{
			std::string temp = m_textures[2];
			m_textures[2] = "";
			widget->Resolve( m_textures, m_fPlayerHealth, pos, size );
			m_textures[2] = temp;
			widget->Update( dt );
		}
		else if ( Image_Widget* widget = dynamic_cast<Image_Widget*>( &m_vWidgets[i] ) )
		{
			widget->Resolve( "Resources\\Textures\\UI\\Board\\Board.png", pos, size );
			widget->Update( dt );
		}
		else if ( Input_Widget* widget = dynamic_cast<Input_Widget*>( &m_vWidgets[i] ) )
		{
			widget->Resolve( m_sKeys, Colors::White, m_textures, m_mouseData, pos, size );
			widget->Update( dt );
		}
		else if ( PageSlider_Widget* widget = dynamic_cast<PageSlider_Widget*>( &m_vWidgets[i] ) )
		{
			widget->Resolve(
				Colour( 10.0f, 10.0f, 10.0f ), Colour( 60.0f, 60.0f, 60.0f ), m_mouseData,
				{ m_vScreenSize.x - 30.0f , m_vScreenSize.y * 0.2f },
				{ 30.0f, m_vScreenSize.y * 0.6f } );
			widget->Update( dt );
			m_fCurrentY = ( m_vScreenSize.y * 0.2f ) - widget->GetPagePos();
			if ( m_fCurrentPY != widget->GetPY() )
			{
				m_fCurrentPY = widget->GetPY();
				m_bLoadFlag = true;
			}
			if ( m_bUpdateSlider )
				widget->SetPageSize( m_vScreenSize.y );
		}
		pos.x += m_vScreenSize.x * offset;
	}

	// Update render box
	m_fBoxPos = { 0.0f, m_vScreenSize.y * 0.1f };
	m_fBoxSize = { m_vScreenSize.x, m_vScreenSize.y * 0.6f };
}

void UIScreen::Draw( VertexShader& vtx, PixelShader& pix, XMMATRIX worldOrtho, TextRenderer* textRenderer )
{
	for ( unsigned int i = 0; i < m_vWidgets.size(); i++ )
	{
		if ( Button_Widget* widget = dynamic_cast<Button_Widget*>( &m_vWidgets[i] ) )
		{
			RENDER_IF_IN_BOX( widget->GetTransform()->GetPosition().y, m_fBoxPos.y, m_fBoxSize.y, widget->Draw( m_pDevice.Get(), m_pContext.Get(), worldOrtho, textRenderer ) );
			Shaders::BindShaders( m_pContext.Get(), vtx, pix );
		}	
		else if ( ColourBlock_Widget* widget = dynamic_cast<ColourBlock_Widget*>( &m_vWidgets[i] ) )
			RENDER_IF_IN_BOX( widget->GetTransform()->GetPosition().y, m_fBoxPos.y, m_fBoxSize.y, widget->Draw( m_pDevice.Get(), m_pContext.Get(), worldOrtho ) );
		else if ( DataSlider_Widget* widget = dynamic_cast<DataSlider_Widget*>( &m_vWidgets[i] ) )
			RENDER_IF_IN_BOX( widget->GetTransform()->GetPosition().y, m_fBoxPos.y, m_fBoxSize.y, widget->Draw( m_pDevice.Get(), m_pContext.Get(), worldOrtho ) );
		else if ( DropDown_Widget* widget = dynamic_cast<DropDown_Widget*>( &m_vWidgets[i] ) )
		{
			RENDER_IF_IN_BOX( widget->GetTransform()->GetPosition().y, m_fBoxPos.y, m_fBoxSize.y, widget->Draw( m_pDevice.Get(), m_pContext.Get(), worldOrtho, textRenderer, vtx, pix ) );
			Shaders::BindShaders( m_pContext.Get(), vtx, pix );
		}
		else if ( EnergyBar_Widget* widget = dynamic_cast<EnergyBar_Widget*>( &m_vWidgets[i] ) )
			RENDER_IF_IN_BOX( widget->GetTransform()->GetPosition().y, m_fBoxPos.y, m_fBoxSize.y, widget->Draw( m_pDevice.Get(), m_pContext.Get(), worldOrtho ) );
		else if ( Image_Widget* widget = dynamic_cast<Image_Widget*>( &m_vWidgets[i] ) )
			RENDER_IF_IN_BOX( widget->GetTransform()->GetPosition().y, m_fBoxPos.y, m_fBoxSize.y, widget->Draw( m_pDevice.Get(), m_pContext.Get(), worldOrtho ) );
		else if ( Input_Widget* widget = dynamic_cast<Input_Widget*>( &m_vWidgets[i] ) )
		{
			RENDER_IF_IN_BOX( widget->GetTransform()->GetPosition().y, m_fBoxPos.y, m_fBoxSize.y, widget->Draw( m_pDevice.Get(), m_pContext.Get(), worldOrtho, textRenderer ) );
			Shaders::BindShaders( m_pContext.Get(), vtx, pix );
		}
		else if ( PageSlider_Widget* widget = dynamic_cast<PageSlider_Widget*>( &m_vWidgets[i] ) )
			widget->Draw( m_pDevice.Get(), m_pContext.Get(), worldOrtho );
	}
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
		m_bUpdateSlider = true;
	}
	break;
	}
}