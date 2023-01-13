#include "stdafx.h"
#include "Player.h"
#include "Graphics.h"
#include <imgui/imgui.h>

Player::Player()
{
	m_vPlayerPos = new Vector2f();
	m_sprite = std::make_shared<Sprite>();
	m_transform = std::make_shared<Transform>( m_sprite );
	m_physics = std::make_shared<Physics>( m_transform );
	m_pProjectileManager = std::make_shared<ProjectileManager>();
	AddToEvent();
}

void Player::Initialize( const Graphics& gfx, ConstantBuffer<Matrices>& mat )
{
	m_sprite->Initialize( gfx.GetDevice(), gfx.GetContext(), Sprite::Type::Player, mat );

	GetProjectileManager()->Initialize(gfx, mat);
}

void Player::Update( const float dt )
{
	m_sprite->Update( dt );
	m_physics->Update( dt );
	m_transform->Update();

	m_vPlayerPos->x = m_transform->GetPosition().x;
	m_vPlayerPos->y = m_transform->GetPosition().y;
	EventSystem::Instance()->AddEvent( EVENTID::PlayerPosition, m_vPlayerPos );
}

void Player::SpawnControlWindow()
{
	bool bFire = false;
	if ( ImGui::Begin( "Player##Window", FALSE, ImGuiWindowFlags_AlwaysAutoResize ) )
	{
		ImGui::Text( std::string( "X: " ).append( std::to_string( m_transform->GetPosition().x ) ).c_str() );
		ImGui::Text( std::string( "Y: " ).append( std::to_string( m_transform->GetPosition().y ) ).c_str() );
		ImGui::Separator();
		bFire = ImGui::Button("FIRE!");
	}

	if (bFire)
		EventSystem::Instance()->AddEvent(EVENTID::PlayerFire, nullptr);
	
	ImGui::End();
}

void Player::AddToEvent() noexcept
{
	EventSystem::Instance()->AddClient( EVENTID::PlayerUp, this );
	EventSystem::Instance()->AddClient( EVENTID::PlayerLeft, this );
	EventSystem::Instance()->AddClient( EVENTID::PlayerDown, this );
	EventSystem::Instance()->AddClient( EVENTID::PlayerRight, this );
}

void Player::HandleEvent( Event* event )
{
	float movementFactor = 10.0f;
	switch ( event->GetEventID() )
	{
	case EVENTID::PlayerUp: m_physics->AddForce( { 0.0f, -movementFactor } ); break;
	case EVENTID::PlayerLeft: m_physics->AddForce( { -movementFactor, 0.0f } ); break;
	case EVENTID::PlayerDown: m_physics->AddForce( { 0.0f, movementFactor } ); break;
	case EVENTID::PlayerRight: m_physics->AddForce( { movementFactor, 0.0f } ); break;
	default: break;
	}
}