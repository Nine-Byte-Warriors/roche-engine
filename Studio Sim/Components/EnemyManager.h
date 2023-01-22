#pragma once
#ifndef ENEMY_MANAGER_H
#define ENEMY_MANAGER_H

#include "EventSystem.h"
#include "Enemy.h"
#include "Graphics.h"

#include <vector>

class EnemyManager : public Listener
{
public:
	EnemyManager();
	~EnemyManager() {};

	void Update(const float dt);
	void AddAgent(const std::shared_ptr<Enemy>& pEnemy);

	void Initialize(Graphics& gfx, ConstantBuffer<Matrices>& mat, Sprite::Type type);
	
	void AddToEvent() noexcept;
	void HandleEvent(Event* event) override;
	
private:
	std::vector<std::shared_ptr<Enemy>> pEnemies;
};

#endif // !ENEMY_MANAGER_H