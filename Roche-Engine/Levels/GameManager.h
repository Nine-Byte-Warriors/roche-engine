#pragma once
#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "AudioEngine.h"

enum class GameState
{
	Win,
	Loss,
	Paused,
	Unpaused
};

enum class Phase
{
	DayPhase,
	NightPhase
};

class GameManager : public Listener
{
public:
	GameManager() { AddToEvent(); };
	~GameManager();

	void Initialize();

	void SetCurrentState(GameState state);
	void SetNextDay();
	void SetPhase(Phase phase);

private:
	GameState m_currentState;
	Phase m_currentPhase;
	int m_currentDay;

	// Inherited via Listener
	void HandleEvent(Event* event) override;
	void AddToEvent() noexcept;

	void DayPhase();
	void NightPhase();
	void UpdateBrigtness();

	float* m_fRedOverlay = new float;
	float* m_fGreenOverlay = new float;
	float* m_fBlueOverlay = new float;
};

#endif // !GAMEMANAGER_H