#include "stdafx.h"
#include "GameManager.h"

GameManager* GameManager::GetInstance()
{
	if (!m_instance)
		m_instance = new GameManager();
	
	return m_instance;
}

void GameManager::SetCurrentState(GameState state)
{
	m_currentState = state;
}

GameState GameManager::GetCurrentState()
{
	return m_currentState;
}

GameManager::GameManager()
{
}

GameManager::~GameManager()
{
}
