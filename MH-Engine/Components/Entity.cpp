#include "stdafx.h"
#include "Entity.h"
#include "Graphics.h"

Entity::Entity(EntityController& entityController, int EntityNum)
{
	m_entityController = &entityController;
	m_iEntityNum = EntityNum;
	UpdateType();

	m_vPosition = new Vector2f();
	m_sprite = std::make_shared<Sprite>();
	m_transform = std::make_shared<Transform>(m_sprite);
	m_physics = std::make_shared<Physics>(m_transform);
	m_agent = std::make_shared<Agent>(m_physics);
	m_collider = std::make_shared<CircleCollider>(m_transform, 32);
	m_inventory = std::make_shared<Inventory>();
	//UpdateColliderShape(); //TODO
	m_projectileManager = std::make_shared<ProjectileManager>();
}

Entity::~Entity()
{
	EventSystem::Instance()->RemoveClient(EVENTID::PlayerUp, this);
	EventSystem::Instance()->RemoveClient(EVENTID::PlayerLeft, this);
	EventSystem::Instance()->RemoveClient(EVENTID::PlayerDown, this);
	EventSystem::Instance()->RemoveClient(EVENTID::PlayerRight, this);
	EventSystem::Instance()->RemoveClient(EVENTID::UpdateInventory, this);
	EventSystem::Instance()->RemoveClient(EVENTID::ChangeSeed, this);
}

void Entity::Initialize(const Graphics& gfx, ConstantBuffer<Matrices>& mat)
{
	m_device = gfx.GetDevice();

	if (m_entityType != EntityType::Projectile)
	{
		std::string texture = m_entityController->GetProjectileBullet(m_iEntityNum)->texture;
		m_projectileManager->InitialiseFromFile(gfx, mat, texture);
	}
	m_sprite->Initialize(gfx.GetDevice(), gfx.GetContext(), m_entityController->GetTexture(m_iEntityNum), mat);

	SetPositionInit();
	SetScaleInit();
	UpdateFrame();
	UpdateBehaviour();
}

void Entity::Update(const float dt)
{
	if (m_entityType == EntityType::Player)
	{
		UpdatePlayer(dt);
		AddToEvent();
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
	EventSystem::Instance()->AddEvent(EVENTID::PlayerPosition, m_vPosition);

	//m_projectileManager->SpawnProjectile(*m_vPosition, 10.0f);
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
	m_sprite->Update(dt);
	m_agent->Update(dt);
	m_transform->Update();
}

void Entity::AddToEvent() noexcept
{
	EventSystem::Instance()->AddClient(EVENTID::PlayerUp, this);
	EventSystem::Instance()->AddClient(EVENTID::PlayerLeft, this);
	EventSystem::Instance()->AddClient(EVENTID::PlayerDown, this);
	EventSystem::Instance()->AddClient(EVENTID::PlayerRight, this);
	EventSystem::Instance()->AddClient(EVENTID::UpdateInventory, this);
	EventSystem::Instance()->AddClient(EVENTID::ChangeSeed, this);
}

void Entity::HandleEvent(Event* event)
{
	switch (event->GetEventID())
	{
	case EVENTID::PlayerUp: m_physics->AddForce({ 0.0f, -m_fSpeed }); break;
	case EVENTID::PlayerLeft: m_physics->AddForce({ -m_fSpeed, 0.0f }); break;
	case EVENTID::PlayerDown: m_physics->AddForce({ 0.0f, m_fSpeed }); break;
	case EVENTID::PlayerRight: m_physics->AddForce({ m_fSpeed, 0.0f }); break;
	case EVENTID::UpdateInventory: m_inventory->UpdateCurrentSeedCount(-1); break;
	case EVENTID::ChangeSeed: m_inventory->IncrementCurrentSeed(); break;
	default: break;
	}
}

void Entity::UpdateFromEntityData(const float dt, bool positionLocked)
{
	if (positionLocked)
	{
		UpdatePosition();
	}
	UpdateScale();
	UpdateFrame();
	UpdateMass();
	UpdateType();
	UpdateBehaviour();
	UpdateSpeed();
	UpdateProjectilePattern();
	UpdateTexture();
}

EntityType Entity::GetEntityType()
{
	return m_entityType;
}

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

	if (m_entityType != EntityType::Projectile)
	{
		m_fBulletScaleX = m_entityController->GetProjectileBullet(m_iEntityNum)->scale[0];
		m_fBulletScaleY = m_entityController->GetProjectileBullet(m_iEntityNum)->scale[1];
		for (int i = 0; i < m_projectileManager->GetProjector().size(); i++)
		{
			m_projectileManager->GetProjector()[i]->GetTransform()->SetScaleInit(m_fBulletScaleX, m_fBulletScaleY);
		}
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

	if (m_entityType != EntityType::Projectile)
	{
		m_fBulletScaleX = m_entityController->GetProjectileBullet(m_iEntityNum)->scale[0];
		m_fBulletScaleY = m_entityController->GetProjectileBullet(m_iEntityNum)->scale[1];
		for (int i = 0; i < m_projectileManager->GetProjector().size(); i++)
		{
			m_projectileManager->GetProjector()[i]->GetTransform()->SetScaleInit(m_fBulletScaleX, m_fBulletScaleY);
		}
	}
}

void Entity::UpdateFrame()
{
	m_iMaxFrameX = m_entityController->GetMaxFrame(m_iEntityNum)[0];
	m_iMaxFrameY = m_entityController->GetMaxFrame(m_iEntityNum)[1];
	m_sprite->SetMaxFrame(m_iMaxFrameX, m_iMaxFrameY);

	if (m_entityType != EntityType::Projectile)
	{
		m_iBulletMaxFrameX = m_entityController->GetProjectileBullet(m_iEntityNum)->maxFrame[0];
		m_iBulletMaxFrameY = m_entityController->GetProjectileBullet(m_iEntityNum)->maxFrame[1];
		for (int i = 0; i < m_projectileManager->GetProjector().size(); i++)
		{
			m_projectileManager->GetProjector()[i]->GetSprite()->SetMaxFrame(m_iBulletMaxFrameX, m_iBulletMaxFrameY);
		}
	}
}

void Entity::UpdateTexture()
{
	m_sTex = m_entityController->GetTexture(m_iEntityNum);
	m_sprite->UpdateTex(m_device, m_sTex);

	if (m_entityType != EntityType::Projectile)
	{
		m_sBulletTex = m_entityController->GetProjectileBullet(m_iEntityNum)->texture;
		for (int i = 0; i < m_projectileManager->GetProjector().size(); i++)
		{
			m_projectileManager->GetProjector()[i]->GetSprite()->UpdateTex(m_device, m_sBulletTex);
		}
	}
}

void Entity::UpdateMass()
{
	m_fMass = m_entityController->GetMass(m_iEntityNum);
	m_physics->SetMass(m_fMass);

	if (m_entityType != EntityType::Projectile)
	{
		m_fBulletMass = m_entityController->GetProjectileBullet(m_iEntityNum)->mass;
		for (int i = 0; i < m_projectileManager->GetProjector().size(); i++)
		{
			m_projectileManager->GetProjector()[i]->GetPhysics()->SetMass(m_fBulletMass);
		}
	}
}

void Entity::UpdateSpeed()
{
	m_fSpeed = m_entityController->GetSpeed(m_iEntityNum);

	if (m_entityType == EntityType::Enemy)
	{
		m_agent->SetSpeed(m_fSpeed);
	}
	//if (m_entityType != EntityType::Projectile)
	//{
	//	m_fBulletSpeed = m_entityController->GetProjectileBullet(m_iEntityNum)->speed;
	//	for (int i = 0; i < m_projectileManager->GetProjector().size(); i++)
	//	{
	//		m_projectileManager->GetProjector()[i]->SetSpeed(m_fBulletSpeed);
	//	}
	//}
}

void Entity::UpdateType()
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
}

void Entity::UpdateBehaviour()
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

void Entity::UpdateProjectilePattern() //TODO
{
	//if (m_entityType != EntityType::Projectile)
	//{
	//	m_sBulletPattern = m_entityController->GetProjectileBullet(m_iEntityNum)->projectilePattern;
	//	if (m_sBulletPattern != "None")
	//	{
	//		static bool temp = true;
	//		if (true)
	//		{
	//			temp = !temp;

	//			auto temp = m_projectileManager->GetProjector().size();
	//			for (int i = 0; i < m_projectileManager->GetProjector().size(); i++)
	//			{

	//				m_projectileManager->UpdatePattern(m_sBulletPattern);
	//				/*m_projectileManager->GetProjector()[i]->*/
	//			}
	//		}
	//	}
	//}
}

void Entity::UpdateColliderShape()
{
	m_sColliderShape = m_entityController->GetColliderShape(m_iEntityNum);
	//TODO add logic to change the shape
}

void Entity::UpdateColliderRadius()
{
	m_fColliderRadiusX = m_entityController->GetColliderRadius(m_iEntityNum)[0];
	m_fColliderRadiusY = m_entityController->GetColliderRadius(m_iEntityNum)[1];

	m_collider->SetRadius(m_fColliderRadiusX);
	//m_collider->SetRadius(m_fColliderRadiusX, m_fColliderRadiusY); //Box
}
