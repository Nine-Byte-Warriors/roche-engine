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
	int inputIdx = 0;
	for ( unsigned int i = 0; i < m_vWidgets.size(); i++ )
	{
		if ( m_vWidgets[i]->GetType() == "Button" )
		{
			m_vWidgets[i]->GetButtonWidget()->Initialize( m_pDevice.Get(), m_pContext.Get(), *m_cbMatrices );
		}
		/*else if ( m_vWidgets[i]->GetType() == "Colour Block" )
		{
			std::shared_ptr<ColourBlock_Widget> colourBlockPtr = std::dynamic_pointer_cast<ColourBlock_Widget>( m_vWidgets[i] );
			colourBlockPtr = std::make_shared<ColourBlock_Widget>();
			colourBlockPtr->Initialize( m_pDevice.Get(), m_pContext.Get(), *m_cbMatrices );
			colourBlockPtr->IntializeWidget( m_vWidgets[i] );
		}
		else if ( m_vWidgets[i]->GetType() == "Data Slider" )
		{
			std::shared_ptr<DataSlider_Widget> dataSliderPtr = std::dynamic_pointer_cast<DataSlider_Widget>( m_vWidgets[i] );
			dataSliderPtr = std::make_shared<DataSlider_Widget>();
			dataSliderPtr->Initialize( m_pDevice.Get(), m_pContext.Get(), *m_cbMatrices );
			dataSliderPtr->IntializeWidget( m_vWidgets[i] );
		}
		else if ( m_vWidgets[i]->GetType() == "Drop Down" )
		{
			std::shared_ptr<DropDown_Widget> dropDownPtr = std::dynamic_pointer_cast<DropDown_Widget>( m_vWidgets[i] );
			dropDownPtr = std::make_shared<DropDown_Widget>();
			dropDownPtr->Initialize( m_pDevice.Get(), m_pContext.Get(), *m_cbMatrices );
			dropDownPtr->IntializeWidget( m_vWidgets[i] );
		}
		else if ( m_vWidgets[i]->GetType() == "Energy Bar" )
		{
			std::shared_ptr<EnergyBar_Widget> energyBarPtr = std::dynamic_pointer_cast<EnergyBar_Widget>( m_vWidgets[i] );
			energyBarPtr = std::make_shared<EnergyBar_Widget>();
			energyBarPtr->Initialize( m_pDevice.Get(), m_pContext.Get(), *m_cbMatrices );
			energyBarPtr->IntializeWidget( m_vWidgets[i] );
		}
		else if ( m_vWidgets[i]->GetType() == "Image" )
		{
			std::shared_ptr<Image_Widget> imagePtr = std::dynamic_pointer_cast<Image_Widget>( m_vWidgets[i] );
			imagePtr = std::make_shared<Image_Widget>();
			imagePtr->Initialize( m_pDevice.Get(), m_pContext.Get(), *m_cbMatrices );
			imagePtr->IntializeWidget( m_vWidgets[i] );
		}
		else if ( m_vWidgets[i]->GetType() == "Input" )
		{
			std::shared_ptr<Input_Widget> inputPtr = std::dynamic_pointer_cast<Input_Widget>( m_vWidgets[i] );
			inputPtr = std::make_shared<Input_Widget>();
			inputPtr->Initialize( m_pDevice.Get(), m_pContext.Get(), *m_cbMatrices, inputIdx );
			inputPtr->IntializeWidget( m_vWidgets[i] );
			inputIdx++;
		}
		else if ( m_vWidgets[i]->GetType() == "Page Slider" )
		{
			std::shared_ptr<PageSlider_Widget> pageSliderPtr = std::dynamic_pointer_cast<PageSlider_Widget>( m_vWidgets[i] );
			pageSliderPtr = std::make_shared<PageSlider_Widget>();
			pageSliderPtr->Initialize( m_pDevice.Get(), m_pContext.Get(), *m_cbMatrices );
			pageSliderPtr->IntializeWidget( m_vWidgets[i] );
			pageSliderPtr->SetPageSize( m_vScreenSize.y );
			m_iPageSliderCount++;
		}*/
	}
}

void UIScreen::Update( const float dt )
{
	if ( !m_mouseData.LPress )
		m_mouseData.Locked = false;

	for ( unsigned int i = 0; i < m_vWidgets.size(); i++ )
	{
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
			else if ( m_vWidgets[i]->GetAction() == "Close" )
			{
				// Quit Game
				if ( m_vWidgets[i]->GetButtonWidget()->Resolve( "Quit Game", Colors::White, m_textures, m_mouseData ) )
					EventSystem::Instance()->AddEvent( EVENTID::QuitGameEvent );
			}
			else
			{
				// Default
				m_vWidgets[i]->GetButtonWidget()->Resolve( "-PlaceHolder-", Colors::White, m_textures, m_mouseData );
			}
			m_vWidgets[i]->GetButtonWidget()->Update( dt );
		}
		/*else if ( m_vWidgets[i]->GetType() == "Colour Block" )
		{
			// Doesn't need actions
			std::shared_ptr<ColourBlock_Widget> colourBlockPtr = std::dynamic_pointer_cast<ColourBlock_Widget>( m_vWidgets[i] );
			colourBlockPtr = std::make_shared<ColourBlock_Widget>();
			colourBlockPtr->Resolve( { 210, 210, 150 } );
			colourBlockPtr->Update( dt );
		}
		else if ( m_vWidgets[i]->GetType() == "Data Slider" )
		{
			std::shared_ptr<DataSlider_Widget> dataSliderPtr = std::dynamic_pointer_cast<DataSlider_Widget>( m_vWidgets[i] );
			dataSliderPtr = std::make_shared<DataSlider_Widget>();
			if ( dataSliderPtr->GetAction() == "Master Volume" )
			{
				// Create a slider that syncs with master volume
			}
			else  if ( dataSliderPtr->GetAction() == "Music Volume" )
			{
				// Create a slider that syncs with musics volume
			}
			else
			{
				// Default
				dataSliderPtr->Resolve( m_iSliderStart,
					"Resources\\Textures\\UI\\Slider\\Slider Background.png",
					"Resources\\Textures\\UI\\Slider\\Control Point.png", m_mouseData );
			}
			dataSliderPtr->Update( dt );
		}
		else if ( m_vWidgets[i]->GetType() == "Drop Down" )
		{
			std::shared_ptr<DropDown_Widget> dropDownPtr = std::dynamic_pointer_cast<DropDown_Widget>( m_vWidgets[i] );
			dropDownPtr = std::make_shared<DropDown_Widget>();
			if ( dropDownPtr->GetAction() == "Resolution" )
			{
				// Create a drop down that allows user to change resolution
			}
			else
			{
				// Default
				std::vector<std::string> vValues = { "True", "False" };
				static std::string sValue = vValues[0];
				dropDownPtr->Resolve( vValues, m_texturesDD, m_texturesDDButton, Colors::White, sValue, m_mouseData );
				if ( dropDownPtr->GetSelected() == "False" )
					sValue = "False";
				else
					sValue = "True";
				dropDownPtr->Update( dt );
			}
		}
		else if ( m_vWidgets[i]->GetType() == "Energy Bar" )
		{
			std::shared_ptr<EnergyBar_Widget> energyBarPtr = std::dynamic_pointer_cast<EnergyBar_Widget>( m_vWidgets[i] );
			energyBarPtr = std::make_shared<EnergyBar_Widget>();
			if ( energyBarPtr->GetAction() == "Player Health" )
			{
				// Bar that displays the player's health
			}
			else if ( energyBarPtr->GetAction() == "Enemy Health" )
			{
				// Bar that displays an enemy's health
			}
			else
			{
				// Default
				static float health = 100.0f;
				std::string temp = m_textures[2];
				m_textures[2] = "";
				energyBarPtr->Resolve( m_textures, health );
				m_textures[2] = temp;
			}
			energyBarPtr->Update( dt );
		}
		else if ( m_vWidgets[i]->GetType() == "Image" )
		{
			std::shared_ptr<Image_Widget> imagePtr = std::dynamic_pointer_cast<Image_Widget>( m_vWidgets[i] );
			imagePtr = std::make_shared<Image_Widget>();
			imagePtr->Resolve( "Resources\\Textures\\UI\\Board\\Board.png" );
			imagePtr->Update( dt );
		}
		else if ( m_vWidgets[i]->GetType() == "Input" )
		{
			std::shared_ptr<Input_Widget> inputPtr = std::dynamic_pointer_cast<Input_Widget>( m_vWidgets[i] );
			inputPtr = std::make_shared<Input_Widget>();
			if ( inputPtr->GetAction() == "Player Name" )
			{
				// Input that allows the user to enter their name
			}
			else
			{
				// Default
				inputPtr->Resolve( m_sKeys, Colors::White, m_textures, m_mouseData, m_iInputIndex );
			}
			inputPtr->Update( dt );
		}
		else if ( m_vWidgets[i]->GetType() == "Page Slider" )
		{
			std::shared_ptr<PageSlider_Widget> pageSliderPtr = std::dynamic_pointer_cast<PageSlider_Widget>( m_vWidgets[i] );
			pageSliderPtr = std::make_shared<PageSlider_Widget>();
			pageSliderPtr->Resolve( Colour( 10.0f, 10.0f, 10.0f ), Colour( 60.0f, 60.0f, 60.0f ), m_mouseData );
			//pageSliderPtr->SetPosition( { m_vScreenSize.x - 30.0f, m_vScreenSize.y * 0.2f } );
			//pageSliderPtr->SetSize( { 30.0f, m_vScreenSize.y * 0.6f } );
			pageSliderPtr->Update( dt );

			m_fCurrentY = ( m_vScreenSize.y * 0.2f ) - pageSliderPtr->GetPagePos();
			if ( m_fCurrentPY != pageSliderPtr->GetPY() )
			{
				m_fCurrentPY = pageSliderPtr->GetPY();
				m_bLoadFlag = true;
			}

			if ( m_bUpdateSlider )
				pageSliderPtr->SetPageSize( m_vScreenSize.y );
		}*/
	}

	// Update render box
	if ( m_iPageSliderCount > 0 )
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
		if ( m_vWidgets[i]->GetType() == "Button" )
		{
			if ( m_vWidgets[i]->GetZIndex() == i )
			{
				if ( !m_vWidgets[i]->GetIsHidden() )
				{
					RENDER_IF_IN_BOX( m_vWidgets[i]->GetButtonWidget()->GetTransform()->GetPosition().y, m_fBoxPos.y, m_fBoxSize.y,
						m_vWidgets[i]->GetButtonWidget()->Draw( m_pDevice.Get(), m_pContext.Get(), worldOrtho, textRenderer ) );
					Shaders::BindShaders( m_pContext.Get(), vtx, pix );
				}
				break;
			}
		}
		/*else if ( m_vWidgets[i]->GetType() == "Colour Block" )
		{
			std::shared_ptr<ColourBlock_Widget> colourBlockPtr = std::dynamic_pointer_cast<ColourBlock_Widget>( m_vWidgets[i] );
			colourBlockPtr = std::make_shared<ColourBlock_Widget>();
			if ( colourBlockPtr->GetZIndex() == i )
			{
				if ( !colourBlockPtr->GetIsHidden() )
				{
					RENDER_IF_IN_BOX( colourBlockPtr->GetTransform()->GetPosition().y, m_fBoxPos.y, m_fBoxSize.y,
						colourBlockPtr->Draw( m_pDevice.Get(), m_pContext.Get(), worldOrtho ) );
				}
				break;
			}
		}
		else if ( m_vWidgets[i]->GetType() == "Data Slider" )
		{
			std::shared_ptr<DataSlider_Widget> dataSliderPtr = std::dynamic_pointer_cast<DataSlider_Widget>( m_vWidgets[i] );
			dataSliderPtr = std::make_shared<DataSlider_Widget>();
			if ( dataSliderPtr->GetZIndex() == i )
			{
				if ( !dataSliderPtr->GetIsHidden() )
				{
					RENDER_IF_IN_BOX( dataSliderPtr->GetTransform()->GetPosition().y, m_fBoxPos.y, m_fBoxSize.y,
						dataSliderPtr->Draw( m_pDevice.Get(), m_pContext.Get(), worldOrtho ) );
				}
				break;
			}
		}
		else if ( m_vWidgets[i]->GetType() == "Drop Down" )
		{
			std::shared_ptr<DropDown_Widget> dropDownPtr = std::dynamic_pointer_cast<DropDown_Widget>( m_vWidgets[i] );
			dropDownPtr = std::make_shared<DropDown_Widget>();
			if ( dropDownPtr->GetZIndex() == i )
			{
				if ( !dropDownPtr->GetIsHidden() )
				{
					RENDER_IF_IN_BOX( dropDownPtr->GetTransform()->GetPosition().y, m_fBoxPos.y, m_fBoxSize.y,
						dropDownPtr->Draw( m_pDevice.Get(), m_pContext.Get(), worldOrtho, textRenderer, vtx, pix ) );
					Shaders::BindShaders( m_pContext.Get(), vtx, pix );
				}
				break;
			}
		}
		else if ( m_vWidgets[i]->GetType() == "Energy Bar" )
		{
			std::shared_ptr<EnergyBar_Widget> energyBarPtr = std::dynamic_pointer_cast<EnergyBar_Widget>( m_vWidgets[i] );
			energyBarPtr = std::make_shared<EnergyBar_Widget>();
			if ( energyBarPtr->GetZIndex() == i )
			{
				if ( !energyBarPtr->GetIsHidden() )
				{
					RENDER_IF_IN_BOX( energyBarPtr->GetTransform()->GetPosition().y, m_fBoxPos.y, m_fBoxSize.y,
						energyBarPtr->Draw( m_pDevice.Get(), m_pContext.Get(), worldOrtho ) );
				}
				break;
			}
		}
		else if ( m_vWidgets[i]->GetType() == "Image" )
		{
			std::shared_ptr<Image_Widget> imagePtr = std::dynamic_pointer_cast<Image_Widget>( m_vWidgets[i] );
			imagePtr = std::make_shared<Image_Widget>();
			if ( imagePtr->GetZIndex() == i )
			{
				if ( !imagePtr->GetIsHidden() )
				{
					RENDER_IF_IN_BOX( imagePtr->GetTransform()->GetPosition().y, m_fBoxPos.y, m_fBoxSize.y,
						imagePtr->Draw( m_pDevice.Get(), m_pContext.Get(), worldOrtho ) );
				}
				break;
			}
		}
		else if ( m_vWidgets[i]->GetType() == "Input" )
		{
			std::shared_ptr<Input_Widget> inputPtr = std::dynamic_pointer_cast<Input_Widget>( m_vWidgets[i] );
			inputPtr = std::make_shared<Input_Widget>();
			if ( inputPtr->GetZIndex() == i )
			{
				if ( !inputPtr->GetIsHidden() )
				{
					RENDER_IF_IN_BOX( inputPtr->GetTransform()->GetPosition().y, m_fBoxPos.y, m_fBoxSize.y,
						inputPtr->Draw( m_pDevice.Get(), m_pContext.Get(), worldOrtho, textRenderer ) );
					Shaders::BindShaders( m_pContext.Get(), vtx, pix );
				}
				break;
			}
		}
		else if ( m_vWidgets[i]->GetType() == "Page Slider" )
		{
			std::shared_ptr<PageSlider_Widget> pageSliderPtr = std::dynamic_pointer_cast<PageSlider_Widget>( m_vWidgets[i] );
			pageSliderPtr = std::make_shared<PageSlider_Widget>();
			if ( pageSliderPtr->GetZIndex() == i )
			{
				if ( !pageSliderPtr->GetIsHidden() )
				{
					RENDER_IF_IN_BOX( pageSliderPtr->GetPosition().y, m_fBoxPos.y, m_fBoxSize.y,
						pageSliderPtr->Draw( m_pDevice.Get(), m_pContext.Get(), worldOrtho ) );
				}
				break;
			}
		}*/
	}
}

void UIScreen::AddToEvent() noexcept
{
	EventSystem::Instance()->AddClient( EVENTID::KeyInput, this );
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
	EventSystem::Instance()->RemoveClient( EVENTID::KeyInput, this );
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
	case EVENTID::KeyInput:	{ m_sKeys = *(std::string*)event->GetData(); } break;
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
		m_bUpdateSlider = true;
	}
	break;
	}
}