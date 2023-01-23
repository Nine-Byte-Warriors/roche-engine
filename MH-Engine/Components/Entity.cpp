#include "stdafx.h"
#include "Entity.h"
#include "Graphics.h"

Entity::Entity(EntityController& entityController, int EntityNum)
{
	m_entityController = &entityController;
	m_iEntityNum = EntityNum;
	SetEntityTypeNumInit();

	m_vPosition = new Vector2f();
	m_sprite = std::make_shared<Sprite>();
	m_transform = std::make_shared<Transform>(m_sprite);
	m_physics = std::make_shared<Physics>(m_transform);
	m_agent = std::make_shared<Agent>(m_physics);
	m_collider = std::make_shared<CircleCollider>(m_transform, 32);
	m_projectileManager = std::make_shared<ProjectileManager>();

	//AddToEvent();
}

void Entity::Initialize(const Graphics& gfx, ConstantBuffer<Matrices>& mat)
{
	m_device = gfx.GetDevice();
	m_sprite->Initialize(gfx.GetDevice(), gfx.GetContext(), m_entityController->GetTexture(m_iEntityNum), mat);
	m_projectileManager->Initialize(gfx, mat);

	SetPositionInit();
	SetScaleInit();
	SetFrameInit();
	SetBehaviourInit();
}

void Entity::Update(const float dt)
{
	//m_sprite->Update(dt);
	//m_agent->Update(dt);
	//m_physics->Update(dt);
	//m_transform->Update();
	//m_projectileManager->Update(dt);

	if (m_entityType == EntityType::Player)
	{
		UpdatePlayer(dt);
	}
	else if (m_entityType == EntityType::Enemy)
	{
		UpdateEnemy(dt);
	}
	else if (m_entityType == EntityType::Projectile)
	{
		UpdateProjectile(dt);
	}
}

void Entity::UpdatePlayer(const float dt)
{
	m_sprite->Update(dt);
	m_physics->Update(dt);
	m_transform->Update();
	m_projectileManager->Update(dt);

	m_vPosition->x = m_transform->GetPosition().x;
	m_vPosition->y = m_transform->GetPosition().y;

	float movementFactor = 10.0f;
	EVENTID eventId = m_entityController->GetEventId();
	switch (eventId)
	{
	case EVENTID::PlayerUp: m_physics->AddForce({ 0.0f, -movementFactor }); break;
	case EVENTID::PlayerLeft: m_physics->AddForce({ -movementFactor, 0.0f }); break;
	case EVENTID::PlayerDown: m_physics->AddForce({ 0.0f, movementFactor }); break;
	case EVENTID::PlayerRight: m_physics->AddForce({ movementFactor, 0.0f }); break;
	default: break;
	}

	//EventSystem::Instance()->AddEvent(EVENTID::PlayerPosition, m_vPosition);
}

void Entity::UpdateEnemy(const float dt)
{
	m_sprite->Update(dt);
	m_agent->Update(dt);
	m_transform->Update();

	m_vPosition->x = m_transform->GetPosition().x;
	m_vPosition->y = m_transform->GetPosition().y;
	EventSystem::Instance()->AddEvent(EVENTID::TargetPosition, m_vPosition); // DEBUG: remove
}

void Entity::UpdateProjectile(const float dt)
{

}

void Entity::UpdateFromEntityData(const float dt, bool positionLocked)
{
	if (positionLocked)
	{
		UpdatePosition();
	}
	UpdateScale();
	UpdateFrame();
	UpdateTexture();
	UpdateMass();
	UpdateType();
	UpdateBehaviour();
}

EntityType Entity::GetEntityType()
{
	return m_entityType;
}

//void Entity::AddToEvent() noexcept
//{
//	if (m_entityType == EntityType::Player)
//	//if (true)
//	{
//		//EventSystem::Instance()->AddClient(EVENTID::PlayerUp, this);
//		//EventSystem::Instance()->AddClient(EVENTID::PlayerLeft, this);
//		//EventSystem::Instance()->AddClient(EVENTID::PlayerDown, this);
//		//EventSystem::Instance()->AddClient(EVENTID::PlayerRight, this);
//	}
//}
//
//void Entity::HandleEvent(Event* event)
//{
//	if (m_entityType == EntityType::Player)
//	{
//		float movementFactor = 10.0f;
//		switch (event->GetEventID())
//		{
//			case EVENTID::PlayerUp: m_physics->AddForce({ 0.0f, -movementFactor }); break;
//			case EVENTID::PlayerLeft: m_physics->AddForce({ -movementFactor, 0.0f }); break;
//			case EVENTID::PlayerDown: m_physics->AddForce({ 0.0f, movementFactor }); break;
//			case EVENTID::PlayerRight: m_physics->AddForce({ movementFactor, 0.0f }); break;
//		default: break;
//		}
//	}
//}

void Entity::SetPositionInit()
{
	m_vPosition->x = m_entityController->GetPosition(m_iEntityNum)[0];
	m_vPosition->y = m_entityController->GetPosition(m_iEntityNum)[1];
	m_transform->SetPositionInit(m_vPosition->x, m_vPosition->y);
	m_transform->SetPosition(m_vPosition->x, m_vPosition->y);
}

void Entity::SetScaleInit()
{
	m_fScaleX = m_entityController->GetScale(m_iEntityNum)[0];
	m_fScaleY = m_entityController->GetScale(m_iEntityNum)[1];
	m_transform->SetScaleInit(m_fScaleX, m_fScaleY);
}

void Entity::SetFrameInit()
{
	m_iMaxFrameX = m_entityController->GetMaxFrame(m_iEntityNum)[0];
	m_iMaxFrameY = m_entityController->GetMaxFrame(m_iEntityNum)[1];
	m_sprite->SetMaxFrame(m_iMaxFrameX, m_iMaxFrameY);
}

void Entity::SetEntityTypeNumInit()
{
	m_sEntityType = m_entityController->GetType(m_iEntityNum);

	if (m_sEntityType == "Player")
	{
		m_entityType = EntityType::Player;
	}
	else if (m_sEntityType == "Enemy")
	{
		m_entityType = EntityType::Enemy;
	}
	else if (m_sEntityType == "Projectile")
	{
		m_entityType = EntityType::Projectile;
	}
	else
	{
		m_entityType = EntityType::Other;
	}
}

void Entity::SetBehaviourInit()
{
	m_sBehaviour = m_entityController->GetBehaviour(m_iEntityNum);

	if (m_sBehaviour == "Idle")
	{
		m_agent->SetBehaviour(AILogic::AIStateTypes::Idle);
	}
	else if (m_sBehaviour == "Seek")
	{
		m_agent->SetBehaviour(AILogic::AIStateTypes::Seek);
	}
	else if (m_sBehaviour == "Flee")
	{
		m_agent->SetBehaviour(AILogic::AIStateTypes::Flee);
	}
	else if (m_sBehaviour == "Patrol")
	{
		m_agent->SetBehaviour(AILogic::AIStateTypes::Patrol);
	}
	else if (m_sBehaviour == "Follow")
	{
		m_agent->SetBehaviour(AILogic::AIStateTypes::Follow);
	}
	else if (m_sBehaviour == "Wander")
	{
		m_agent->SetBehaviour(AILogic::AIStateTypes::Wander);
	}
}

void Entity::UpdatePosition()
{
	m_vPosition->x = m_entityController->GetPosition(m_iEntityNum)[0];
	m_vPosition->y = m_entityController->GetPosition(m_iEntityNum)[1];
	m_transform->SetPosition(m_vPosition->x, m_vPosition->y);
}

void Entity::UpdateScale()
{
	m_fScaleX = m_entityController->GetScale(m_iEntityNum)[0];
	m_fScaleY = m_entityController->GetScale(m_iEntityNum)[1];
	m_transform->SetScale(m_fScaleX, m_fScaleY);

	m_collider->SetRadius(m_fScaleX);
}

void Entity::UpdateFrame()
{
	m_iMaxFrameX = m_entityController->GetMaxFrame(m_iEntityNum)[0];
	m_iMaxFrameY = m_entityController->GetMaxFrame(m_iEntityNum)[1];
	m_sprite->SetMaxFrame(m_iMaxFrameX, m_iMaxFrameY);
}

void Entity::UpdateTexture()
{
	m_sTex = m_entityController->GetTexture(m_iEntityNum);
	m_sprite->UpdateTex(m_device, m_sTex);
}

void Entity::UpdateMass()
{
	m_fMass = m_entityController->GetMass(m_iEntityNum);
	m_physics->SetMass(m_fMass);
}

void Entity::UpdateType()
{
	SetEntityTypeNumInit();
}

void Entity::UpdateBehaviour()
{
	SetBehaviourInit();
}
