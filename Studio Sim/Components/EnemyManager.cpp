#include "stdafx.h"
#include "EnemyManager.h"

EnemyManager::EnemyManager()
{
	AddToEvent();
}

void EnemyManager::AddEnemy(const std::shared_ptr<Enemy>& pEnemy)
{
	m_vecEnemies.push_back(pEnemy);
}

void EnemyManager::Update(const float dt)
{
	for (std::shared_ptr<Enemy>& pEnemy : m_vecEnemies)
		pEnemy->Update(dt);
}

void EnemyManager::Initialize(Graphics& gfx, ConstantBuffer<Matrices>& mat, Sprite::Type type)
{
	for (std::shared_ptr<Enemy>& pEnemy : m_vecEnemies)
		pEnemy->Initialize(gfx, mat, type);
}

void EnemyManager::Render(Graphics& gfx, XMMATRIX& mat)
{
	for (std::shared_ptr<Enemy>& pEnemy : m_vecEnemies)
		pEnemy->Render(gfx, mat);
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