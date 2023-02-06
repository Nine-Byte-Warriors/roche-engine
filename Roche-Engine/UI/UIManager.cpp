#include "stdafx.h"
#include "UIManager.h"
#include "UIScreen.h"
#include "Graphics.h"

void UIManager::Initialize( const Graphics& gfx, ConstantBuffer<Matrices>* mat, const std::vector<std::vector<std::shared_ptr<Widget>>>& widgets, Health& health )
{
	m_vWindowSize = { (float)gfx.GetWidth(), (float)gfx.GetHeight() };
	int index = 0;
	for ( auto const& UIItem : m_mUiList )
	{
		UIItem.second->SetScreenSize( m_vWindowSize );
		UIItem.second->Initialize( gfx, mat, widgets[index], health );
		index++;
	}
}

void UIManager::Update( const float dt )
{
	int index = 0;
	for ( auto const& UIItem : m_mUiList )
	{
		bool bToDraw = false;
		for ( int i = 0; i < m_vUiToDraw.size(); i++ )
			if ( UIItem.first == m_vUiToDraw[i] )
				bToDraw = true;

		if ( bToDraw )
			UIItem.second->Update( dt );
		index++;
	}
}

void UIManager::Draw( VertexShader vtx, PixelShader pix, XMMATRIX worldOrtho, TextRenderer* textRenderer )
{
	for ( auto const& UIItem : m_mUiList )
	{
		bool bToDraw = false;
		for ( int i = 0; i < m_vUiToDraw.size(); i++ )
			if ( UIItem.first == m_vUiToDraw[i] )
				bToDraw = true;

		if ( bToDraw )
			UIItem.second->Draw( vtx, pix, worldOrtho, textRenderer );
	}
}

std::shared_ptr<UIScreen> UIManager::GetCustomUI( const std::string& name )
{
	for ( auto const& UIItem : m_mUiList )
		if ( UIItem.first == name )
			return m_mUiList[name];

	return nullptr;
}

void UIManager::AddUI( const std::shared_ptr<UIScreen>& newUI, const std::string& name )
{
	// Check if it is in list
	bool bToAdd = true;
	for ( auto const& UIItem : m_mUiList )
	{
		if ( UIItem.first == name )
		{
			bToAdd = false;
			break;
		}
	}
	if ( bToAdd )
	{
		m_mUiList[name] = newUI;
		m_vUiToDraw.push_back( name );
	}
}

void UIManager::RemoveUI( const std::string& name )
{
	for ( auto i = m_mUiList.begin(); i != m_mUiList.end(); i++ )
	{
		if ( i->first == name )
		{
			// Delete the client in question
			// No need to keep going since its a unique and more cant exist
			i = m_mUiList.erase( i );
			break;
		}
	}
}

void UIManager::RemoveAllUI()
{
	m_mUiList.clear();
}

void UIManager::HandleEvent( Event* event )
{
	switch ( event->GetEventID() )
	{
	case EVENTID::WindowSizeChangeEvent: { m_vWindowSize = *static_cast<XMFLOAT2*>( event->GetData() ); } break;
	case EVENTID::RemoveUIItemEvent: { RemoveUI( *static_cast<std::string*>( event->GetData() ) ); } break;
	case EVENTID::StartGame:
	{
		RemoveAllUI();
		int* levelNumber = new int(0);
		EventSystem::Instance()->AddEvent(EVENTID::GameLevelChangeEvent, levelNumber);
		// TO DO: Load intiial hud day UI after loading, GameStartedEvent to listen to?
	}
	break;
	case EVENTID::OpenCredits:
	{
		HideAllUI();
		ShowUI("Credits_Widgets");
	}
	break;
	case EVENTID::OpenSettings:
	{
		HideAllUI();
		ShowUI("Settings_Widgets");
	}
	break;
	case EVENTID::PauseGame:
	{
		HideAllUI();
		ShowUI("Pause_Widgets");
	}
	break;
	case EVENTID::ResumeGame:
	{
		HideAllUI();
		if (m_currentGamePhase == Phase::DayPhase) {
			ShowUI("HUD_Day");
		}
		else {
			ShowUI("HUD_Night");
		}
	}
	break;
	case EVENTID::Back:
	{
		HideAllUI();
		ShowUI("Pause_Widgets");
		ShowUI("Menu_Widgets");
	}
	break;
	case EVENTID::CurrentPhase: { m_currentGamePhase = *static_cast<Phase*>(event->GetData()); } break;
	case EVENTID::BackToMainMenu:
	{
		RemoveAllUI();
		int* levelNumber = new int(2);
		EventSystem::Instance()->AddEvent(EVENTID::GameLevelChangeEvent, levelNumber);
	}
	break;
	case EVENTID::WinWindow:
	{
		HideAllUI();
		ShowUI("Win_Widgets");
	}
	break;
	case EVENTID::LossWindow:
	{
		HideAllUI();
		ShowUI("Loss_Widgets");
	}
	break;
	case EVENTID::GameRestartEvent:
		HideAllUI();
		ShowUI("HUD_Day");
		int* levelNumber = new int(1);
		EventSystem::Instance()->AddEvent(EVENTID::GameLevelChangeEvent, levelNumber);
	break;
	}

}

void UIManager::AddToEvent() noexcept
{
	EventSystem::Instance()->AddClient( EVENTID::WindowSizeChangeEvent, this );
	EventSystem::Instance()->AddClient( EVENTID::RemoveUIItemEvent, this );
	EventSystem::Instance()->AddClient(EVENTID::StartGame, this);
	EventSystem::Instance()->AddClient(EVENTID::OpenCredits, this);
	EventSystem::Instance()->AddClient(EVENTID::OpenSettings, this);
	EventSystem::Instance()->AddClient(EVENTID::PauseGame, this);
	EventSystem::Instance()->AddClient(EVENTID::ResumeGame, this);
	EventSystem::Instance()->AddClient(EVENTID::Back, this);
	EventSystem::Instance()->AddClient(EVENTID::CurrentPhase, this);
	EventSystem::Instance()->AddClient(EVENTID::WinWindow, this);
	EventSystem::Instance()->AddClient(EVENTID::LossWindow, this);
	EventSystem::Instance()->AddClient(EVENTID::GameRestartEvent, this);
}

void UIManager::RemoveFromEvent() noexcept
{
	EventSystem::Instance()->RemoveClient( EVENTID::WindowSizeChangeEvent, this );
	EventSystem::Instance()->RemoveClient( EVENTID::RemoveUIItemEvent, this );
	EventSystem::Instance()->RemoveClient(EVENTID::StartGame, this);
	EventSystem::Instance()->RemoveClient(EVENTID::OpenCredits, this);
	EventSystem::Instance()->RemoveClient(EVENTID::OpenSettings, this);
	EventSystem::Instance()->RemoveClient(EVENTID::PauseGame, this);
	EventSystem::Instance()->RemoveClient(EVENTID::ResumeGame, this);
	EventSystem::Instance()->RemoveClient(EVENTID::Back, this);
	EventSystem::Instance()->RemoveClient(EVENTID::CurrentPhase, this);
	EventSystem::Instance()->RemoveClient(EVENTID::WinWindow, this);
	EventSystem::Instance()->RemoveClient(EVENTID::LossWindow, this);
	EventSystem::Instance()->RemoveClient(EVENTID::GameRestartEvent, this);
}

void UIManager::HideAllUI()
{
	for ( auto const& UIItem : m_mUiList )
	{
		std::vector<std::string>::iterator iter = m_vUiToDraw.begin();
		while ( iter != m_vUiToDraw.end() )
		{
			if ( *iter == UIItem.first )
				iter = m_vUiToDraw.erase( iter );
			else
				++iter;
		}
	}
}

void UIManager::ShowAllUI()
{
	for ( auto const& UIItem : m_mUiList )
		m_vUiToDraw.push_back( UIItem.first );
}

void UIManager::ShowUI( const std::string& name )
{
	m_vUiToDraw.push_back( name );
}

void UIManager::HideUI( const std::string& name )
{
	std::vector<std::string>::iterator iter = m_vUiToDraw.begin();
	while ( iter != m_vUiToDraw.end() )
	{
		if ( *iter == name )
			iter = m_vUiToDraw.erase( iter );
		else
			++iter;
	}
}