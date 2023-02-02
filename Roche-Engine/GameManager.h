#pragma once
class GameManager
{
public:
	enum GameState
	{
		DayPhase,
		NightPhase,
		Win,
		Loss,
		Paused,
		Quit
	};
	
	static GameManager* GetInstance();

	void SetCurrentState(GameState state);
	GameState GetCurrentState();

private:
	static GameManager* m_instance;

	GameState m_currentState;
	
	GameManager();
	~GameManager();
};