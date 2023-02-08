#pragma once
#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

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
	void Initialize();

	void SetCurrentState(GameState state);
	void SetNextDay();
	void SetPhase(Phase phase);

private:
	GameState m_currentState;
	Phase m_currentPhase;
	int m_currentDay;

	GameManager();
	~GameManager();

	// Inherited via Listener
	virtual void HandleEvent(Event* event) override;
	void AddToEvent() noexcept;

	void DayPhase();
	void NightPhase();
	void UpdateBrigtness();

	float* m_fRedOverlay = new float;
	float* m_fGreenOverlay = new float;
	float* m_fBlueOverlay = new float;
};

#endif // !GAMEMANAGER_H