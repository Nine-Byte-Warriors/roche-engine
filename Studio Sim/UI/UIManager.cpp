#include "stdafx.h"
#include "UIManager.h"
#include "MainMenu_UI.h"
#include "Graphics.h"

void UIManager::Initialize( const Graphics& gfx, ConstantBuffer<Matrices>* mat )
{
	for ( auto const& UIItem : m_mUiList )
	{
		UIItem.second->SetSizeOfScreen( m_vWindowSize );
		UIItem.second->Initialize( gfx, mat );
	}
}

void UIManager::Update( float dt )
{
	for ( auto const& UIItem : m_mUiList )
	{
		bool ToDraw = false;
		for ( int i = 0; i < m_vUiToDraw.size(); i++ )
			if ( UIItem.first == m_vUiToDraw[i] )
				ToDraw = true;

		if ( ToDraw )
			UIItem.second->Update( dt );
	}
}

void UIManager::Draw( XMMATRIX worldOrtho )
{
	for ( auto const& UIItem : m_mUiList )
	{
		bool ToDraw = false;
		for ( int i = 0; i < m_vUiToDraw.size(); i++ )
			if ( UIItem.first == m_vUiToDraw[i] )
				ToDraw = true;

		if ( ToDraw )
			UIItem.second->Draw( XMLoadFloat4x4( &WorldOrthMatrix ) );
	}
}

std::shared_ptr<UI> UIManager::GetCustomUI( const std::string& name )
{
	for ( auto const& UIItem : m_mUiList )
		if ( UIItem.first == name )
			return m_mUiList[name];

	return nullptr;
}

void UIManager::AddUI( std::shared_ptr<UI> newUI, const std::string& name )
{
	// Check if it is in list
	bool ToAdd = true;
	for ( auto const& UIItem : m_mUiList )
	{
		if ( UIItem.first == name )
		{
			ToAdd = false;
			break;
		}
	}
	if ( ToAdd )
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
	case EVENTID::WorldOrthMatrixEvent:
	{
		WorldOrthMatrix = *(XMFLOAT4X4*)event->GetData();
	}
	break;

	case EVENTID::WindowSizeChangeEvent:
	{
		m_vWindowSize = *static_cast<XMFLOAT2*>( event->GetData() );
	}
	break;

	case EVENTID::RemoveUIItemEvent:
	{
		RemoveUI( *static_cast<std::string*>( event->GetData() ) );
	}
	break;
	}
}

void UIManager::AddToEvent()
{
	EventSystem::Instance()->AddClient( EVENTID::WorldOrthMatrixEvent, this );
	EventSystem::Instance()->AddClient( EVENTID::WindowSizeChangeEvent, this );
	EventSystem::Instance()->AddClient( EVENTID::RemoveUIItemEvent, this );
}

void UIManager::RemoveFromEvent()
{
	EventSystem::Instance()->RemoveClient( EVENTID::WorldOrthMatrixEvent, this );
	EventSystem::Instance()->RemoveClient( EVENTID::WindowSizeChangeEvent, this );
	EventSystem::Instance()->RemoveClient( EVENTID::RemoveUIItemEvent, this );
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