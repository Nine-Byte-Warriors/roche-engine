#include "stdafx.h"
#include "EnemyManager.h"

EnemyManager::EnemyManager()
{
	AddToEvent();
}

void EnemyManager::AddAgent(const std::shared_ptr<Enemy>& pEnemy)
{
	pEnemies.push_back(pEnemy);
}

void EnemyManager::Update(const float dt)
{
	for (auto& enemy : pEnemies)
	{
		enemy->Update(dt);
	}
}

void EnemyManager::Initialize(Graphics& gfx, ConstantBuffer<Matrices>& mat, Sprite::Type type)
{
	for (auto& enemy : pEnemies)
	{
		enemy->Initialize(gfx, mat, type);
	}
}

void EnemyManager::AddToEvent() noexcept
{
	//EventSystem::Instance()->AddListener(EVENTID::TargetPosition, this);
}

void EnemyManager::HandleEvent(Event* event)
{
	//if (event->GetEventID() == EVENTID::TargetPosition)
	//{
	//	Vector2f* vTargetPos = static_cast<Vector2f*>(event->GetData());
	//	for (auto& pEnemy : pEnemies)
	//	{
	//		pEnemy->SetTargetPos(*vTargetPos);
	//	}
	//}
}