#include "stdafx.h"
#include "Entity.h"
#include "Graphics.h"

Entity::Entity(EntityController& entityController, int EntityNum)
{
	m_entityController = &entityController;
	m_iEntityNum = EntityNum;

	m_vPosition = new Vector2f();
	m_sprite = std::make_shared<Sprite>();
	m_transform = std::make_shared<Transform>(m_sprite);
	m_physics = std::make_shared<Physics>(m_transform);
	m_projectileManager = std::make_shared<ProjectileManager>();
	m_collider = std::make_shared<BoxCollider>(m_transform, 1, 1, 32, 32);	

	AddToEvent();
}

void Entity::Initialize(const Graphics& gfx, ConstantBuffer<Matrices>& mat)
{
	m_sprite->Initialize(gfx.GetDevice(), gfx.GetContext(), m_entityController->GetTexture(m_iEntityNum), mat);
	m_projectileManager->Initialize(gfx, mat);

	SetPositionInit();
	SetScaleInit();
}

void Entity::Update(const float dt)
{
	m_sprite->Update(dt);
	m_physics->Update(dt);
	m_transform->Update();
	m_projectileManager->Update(dt);

	//m_vPlayerPos->x = m_transform->GetPosition().x;
	//m_vPlayerPos->y = m_transform->GetPosition().y;
	//EventSystem::Instance()->AddEvent(EVENTID::PlayerPosition, m_vPlayerPos);
}

void Entity::AddToEvent() noexcept
{
	if (m_entityController->GetType(m_iEntityNum) == "Player")
	{
		EventSystem::Instance()->AddClient(EVENTID::PlayerUp, this);
		EventSystem::Instance()->AddClient(EVENTID::PlayerLeft, this);
		EventSystem::Instance()->AddClient(EVENTID::PlayerDown, this);
		EventSystem::Instance()->AddClient(EVENTID::PlayerRight, this);
	}
}

void Entity::HandleEvent(Event* event)
{
	float movementFactor = 10.0f;
	switch (event->GetEventID())
	{
	case EVENTID::PlayerUp: m_physics->AddForce({ 0.0f, -movementFactor }); break;
	case EVENTID::PlayerLeft: m_physics->AddForce({ -movementFactor, 0.0f }); break;
	case EVENTID::PlayerDown: m_physics->AddForce({ 0.0f, movementFactor }); break;
	case EVENTID::PlayerRight: m_physics->AddForce({ movementFactor, 0.0f }); break;
	default: break;
	}
}

void Entity::SetPositionInit()
{
	m_vPosition->x = m_entityController->GetPosition(m_iEntityNum)[0];;
	m_vPosition->y = m_entityController->GetPosition(m_iEntityNum)[1];;
	m_transform->SetPositionInit(m_vPosition->x, m_vPosition->y);
}

void Entity::SetScaleInit()
{
	float widthScale = m_entityController->GetScale(m_iEntityNum)[0];
	float heightScale = m_entityController->GetScale(m_iEntityNum)[1];
	m_transform->SetScaleInit(widthScale, heightScale);
}
