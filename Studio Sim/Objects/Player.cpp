#include "stdafx.h"
#include "Player.h"
#include <imgui/imgui.h>

Player::Player()
{
	m_sprite = std::make_shared<Sprite>();
	
	m_transform = std::make_shared<Transform>();
	m_transform->SetPosition( XMFLOAT3( 0.0f, 0.0f, 0.0f ) );
	m_transform->SetRotation( XMFLOAT3( 0.0f, 0.0f, 0.0f ) );
	m_transform->SetScale( XMFLOAT3( 1.0f, 1.0f, 1.0f ) );
	m_transform->Update(); // Initial updated required

	m_physics = std::make_shared<Physics>( m_transform );

	AddToEvent();
}

void Player::Update( const float dt )
{
	m_sprite->Update( dt );
	m_physics->Update( dt );
	m_transform->Update();
}

void Player::SpawnControlWindow()
{
	if ( ImGui::Begin( "Player", FALSE, ImGuiWindowFlags_AlwaysAutoResize ) )
	{
		ImGui::Text( std::string( "X: " ).append( std::to_string( m_transform->GetPositionFloat3().x ) ).c_str() );
		ImGui::Text( std::string( "Y: " ).append( std::to_string( m_transform->GetPositionFloat3().y ) ).c_str() );
		ImGui::Text( std::string( "Z: " ).append( std::to_string( m_transform->GetPositionFloat3().z ) ).c_str() );
	}
	ImGui::End();
}

void Player::AddToEvent()
{
	EventSystem::Instance()->AddClient( EVENTID::PlayerUp, this );
	EventSystem::Instance()->AddClient( EVENTID::PlayerLeft, this );
	EventSystem::Instance()->AddClient( EVENTID::PlayerDown, this );
	EventSystem::Instance()->AddClient( EVENTID::PlayerRight, this );
}

void Player::HandleEvent( Event* event )
{
	switch ( event->GetEventID() )
	{
	case EVENTID::PlayerUp:    m_physics->AddThrust( {  0.0f,	10.0f }, 0.05f ); break;
	case EVENTID::PlayerLeft:  m_physics->AddThrust( { -10.0f,  0.0f },	 0.05f ); break;
	case EVENTID::PlayerDown:  m_physics->AddThrust( {	 0.0f, -10.0f }, 0.05f ); break;
	case EVENTID::PlayerRight: m_physics->AddThrust( {  10.0f,  0.0f },  0.05f ); break;
	default: break;
	}
}