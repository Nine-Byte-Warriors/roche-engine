#include "stdafx.h"
#include "Entity.h"
#include "Graphics.h"
#include <EnemyController.h>

#define PI 3.1415

Entity::Entity(EntityController& entityController, int EntityNum)
{
	m_agent = nullptr;

	m_emitter = nullptr;

	m_colliderCircle = nullptr;
	m_colliderBox = nullptr;

	m_pController = nullptr;
	m_inventory = nullptr;

	m_vPosition = new Vector2f();

	m_entityController = &entityController;
	m_iEntityNum = EntityNum;

	SetComponents();
}

Entity::~Entity()
{
}

void Entity::SetComponents()
{
	m_sprite = std::make_shared<Sprite>();
	m_transform = std::make_shared<Transform>( m_sprite );
	m_physics = std::make_shared<Physics>(m_transform);
	m_health = std::make_shared<Health>( GetType(), m_iEntityNum );
	m_health->SetHealth( m_entityController->GetHealth( m_iEntityNum ) );

	if (m_entityController->HasAI(m_iEntityNum))
	{
		m_agent = std::make_shared<Agent>(m_physics);
	}

	if (m_entityController->HasProjectileSystem(m_iEntityNum))
	{
		m_vecProjectileManagers = ProjectileManager::GenerateManagers(m_entityController->GetProjectilePattern(m_iEntityNum));
		m_emitter = std::make_shared<Emitter>(m_vecProjectileManagers, 0.01f);
	}

	if (m_entityController->HasCollider(m_iEntityNum))
	{
		bool trigger = m_entityController->GetColliderTrigger(m_iEntityNum);
		m_colliderCircle = std::make_shared<CircleCollider> (m_transform, m_sprite, trigger, m_iEntityNum, m_sEntityType, 32);
		m_colliderBox = std::make_shared<BoxCollider>(m_transform, m_sprite, trigger, m_iEntityNum, m_sEntityType, 32, 32);
	}
	
	if (GetType() == "Player")
	{
		m_pController = std::make_shared<PlayerController>(m_physics, m_sprite, m_emitter);
		m_inventory = std::make_shared<Inventory>();
	}

	if (GetType() == "Enemy")
	{
		m_pController = std::make_shared<EnemyController>(m_physics, m_sprite, m_emitter);
	}
}

void Entity::Initialize(const Graphics& gfx, ConstantBuffer<Matrices>& mat)
{
	m_device = gfx.GetDevice();

	SetProjectileManagerInit(gfx, mat);

	m_sprite->Initialize(gfx.GetDevice(), gfx.GetContext(), m_entityController->GetTexture(m_iEntityNum), mat);

	SetPositionInit();
	SetScaleInit();
	UpdateRotation();
	UpdateBehaviour();
	UpdateColliderRadius();
	SetAnimation();
	UpdateRowsColumns();
}

void Entity::SetProjectileManagerInit(const Graphics& gfx, ConstantBuffer<Matrices>& mat)
{
	if (m_entityController->HasProjectileBullet(m_iEntityNum) && m_entityController->HasProjectileSystem(m_iEntityNum))
	{
		std::string texture = m_entityController->GetProjectileBullet(m_iEntityNum)->texture;

		for (std::shared_ptr<ProjectileManager>& pManager : m_vecProjectileManagers)
			pManager->InitialiseFromFile(gfx, mat, texture);
	}
}

void Entity::Update(const float dt)
{
	m_sprite->Update(dt);
	m_transform->Update();
	m_physics->Update(dt);

	if (m_entityController->HasAI(m_iEntityNum))
		m_agent->Update(dt);

	if (m_entityController->HasProjectileSystem(m_iEntityNum))
	{
		for (std::shared_ptr<ProjectileManager>& pManager : m_vecProjectileManagers)
			pManager->Update(dt);
		m_emitter->Update(dt);
	}

	if (m_pController)
		m_pController->Update(dt);
}

std::string Entity::GetType()
{
	return m_entityController->GetType(m_iEntityNum);
}

void Entity::UpdateFromEntityData(const float dt, bool positionLocked)
{
	if (positionLocked)
		UpdatePosition();

	UpdateScale();
	UpdateRotation();
	UpdateMass();
	UpdateBehaviour();
	UpdateSpeed();
	UpdateProjectilePattern();
	UpdateTexture();
	UpdateColliderRadius();
	UpdateAnimation();
	UpdateRowsColumns();
	UpdateAudio();
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
	m_sprite->SetWidthHeight( m_fScaleX, m_fScaleY );

	if (m_entityController->HasProjectileBullet(m_iEntityNum))
	{
		m_fBulletScaleX = m_entityController->GetProjectileBullet(m_iEntityNum)->scale[0];
		m_fBulletScaleY = m_entityController->GetProjectileBullet(m_iEntityNum)->scale[1];

		for ( std::shared_ptr<ProjectileManager>& pManager : m_vecProjectileManagers )
			for ( std::shared_ptr<Projectile> pProjectile : pManager->GetProjector() )
				pProjectile->GetSprite()->SetWidthHeight( m_fBulletScaleX, m_fBulletScaleY );
	}
}

void Entity::UpdateRowsColumns()
{
	m_iRows = m_entityController->GetRows(m_iEntityNum);
	m_iColumns = m_entityController->GetColumns(m_iEntityNum);
	m_sprite->SetRows(m_iRows);
	m_sprite->SetColumns(m_iColumns);
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
	m_sprite->SetWidthHeight( m_fScaleX, m_fScaleY );

	if (m_entityController->HasProjectileBullet(m_iEntityNum))
	{
		m_fBulletScaleX = m_entityController->GetProjectileBullet(m_iEntityNum)->scale[0];
		m_fBulletScaleY = m_entityController->GetProjectileBullet(m_iEntityNum)->scale[1];
		for ( std::shared_ptr<ProjectileManager> pManager : m_vecProjectileManagers )
			for ( std::shared_ptr<Projectile> pProjectile : pManager->GetProjector() )
				pProjectile->GetSprite()->SetWidthHeight( m_fBulletScaleX, m_fBulletScaleY );
	}
}

void Entity::UpdateAnimation()
{
	if (m_entityController->HasAnimation(m_iEntityNum))
	{
		for (int i = 0; i < m_animation.GetAnimationTypeSize(); i++)
		{
			if (m_animation.GetAnimationTypeName(i) == m_entityController->GetAnimationType(m_iEntityNum))
			{
				m_iMaxFrameX = m_animation.GetFrameCount()[i];
				m_iCurFrameY = i;
				m_sprite->UpdateFrameTime(m_animation.GetFrameTiming(i));
			}
		}

		m_sprite->SetMaxFrame(m_iMaxFrameX, m_iMaxFrameY);
		//m_sprite->SetCurFrameY(m_iCurFrameY);

		if (m_entityController->HasProjectileBullet(m_iEntityNum))
		{
			m_iBulletMaxFrameX = m_entityController->GetProjectileBullet(m_iEntityNum)->maxFrame[0];
			m_iBulletMaxFrameY = m_entityController->GetProjectileBullet(m_iEntityNum)->maxFrame[1];
			for (std::shared_ptr<ProjectileManager> pManager : m_vecProjectileManagers)
			{
				for (std::shared_ptr<Projectile> pProjectile : pManager->GetProjector())
					pProjectile->GetSprite()->SetMaxFrame(m_iBulletMaxFrameX, m_iBulletMaxFrameY);
			}
		}
	}
}

void Entity::UpdateRotation()
{
	m_fRotation = m_entityController->GetRotation(m_iEntityNum) * PI / 4;
	m_transform->SetRotation(m_fRotation);
}

void Entity::UpdateTexture()
{
	m_sTex = m_entityController->GetTexture(m_iEntityNum);
	m_sprite->UpdateTex(m_device, m_sTex);

	if (m_entityController->HasProjectileBullet(m_iEntityNum))
	{
		m_sBulletTex = m_entityController->GetProjectileBullet(m_iEntityNum)->texture;
		for (std::shared_ptr<ProjectileManager> pManager : m_vecProjectileManagers)
		{
			for (std::shared_ptr<Projectile> pProjectile : pManager->GetProjector())
				pProjectile->GetSprite()->UpdateTex(m_device, m_sBulletTex);
		}
	}
}

void Entity::UpdateMass()
{
	if (m_agent != nullptr)
	{
		m_fMass = m_entityController->GetMass(m_iEntityNum);
		m_physics->SetMass(m_fMass);

		if (m_entityController->HasProjectileBullet(m_iEntityNum))
		{
			m_fBulletMass = m_entityController->GetProjectileBullet(m_iEntityNum)->mass;
			for (std::shared_ptr<ProjectileManager> pManager : m_vecProjectileManagers)
			{
				for (std::shared_ptr<Projectile> pProjectile : pManager->GetProjector())
					pProjectile->GetPhysics()->SetMass(m_fBulletMass);
			}
		}
	}
}

void Entity::UpdateSpeed()
{
	m_fSpeed = m_entityController->GetSpeed(m_iEntityNum);

	if (m_entityController->HasAI(m_iEntityNum) && m_agent != nullptr)
	{
		m_agent->SetSpeed(m_fSpeed);
	}
}

void Entity::UpdateBehaviour()
{
	if (m_entityController->HasAI(m_iEntityNum) && m_agent != nullptr)
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

void Entity::UpdateColliderRadius()
{
	if (m_entityController->HasCollider(m_iEntityNum) && m_colliderCircle != nullptr)
	{
		m_fColliderRadiusX = m_entityController->GetColliderRadius(m_iEntityNum)[0];
		m_fColliderRadiusY = m_entityController->GetColliderRadius(m_iEntityNum)[1];

		if (m_entityController->GetColliderShape(m_iEntityNum) == "Circle")
		{
			m_colliderCircle->SetRadius(m_fColliderRadiusX);
			m_colliderBox->SetWidth(0);
			m_colliderBox->SetHeight(0);
		}
		else if (m_entityController->GetColliderShape(m_iEntityNum) == "Box")
		{
			m_colliderBox->SetWidth(m_fColliderRadiusX);
			m_colliderBox->SetHeight(m_fColliderRadiusY);
			m_colliderCircle->SetRadius(0);
		}
	}
}

void Entity::UpdateAudio()
{
	if(m_entityController->HasAudio(m_iEntityNum))
		m_sSoundBankName = m_entityController->GetSoundBankName(m_iEntityNum);
}

void Entity::UpdateEntityNum(int num)
{
	m_iEntityNum = num;
}

void Entity::SetAnimation()
{
	if (m_entityController->GetAnimationFile(m_iEntityNum) != "None" && m_entityController->HasAnimation(m_iEntityNum))
	{
		m_animation.LoadEntityAnimation(m_entityController->GetAnimationFile(m_iEntityNum));
		m_iMaxFrameY = m_animation.GetFrameCount().size();
		UpdateAnimation();
	}
}

void Entity::CheckAliveStatus()
{
	if (m_fEntityHealth <= 0.0)
	{
		m_entityController->SetDead(m_iEntityNum);
	}
}



