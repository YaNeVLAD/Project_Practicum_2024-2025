#pragma once

#include <string>

class GameConfig
{
public:
	enum class PlayerType
	{
		First,
		Second,
	};

	static GameConfig* GetInstance()
	{
		if (mInstance == nullptr)
		{
			mInstance = new GameConfig();
		}
		return mInstance;
	}

	size_t killedEnemies = 0;
	size_t killedBosses = 0;
	size_t maxBosses = 1;
	PlayerType playerType = PlayerType::First;

private:
	static GameConfig* mInstance;

	GameConfig(const GameConfig&) = delete;
	GameConfig operator=(const GameConfig&) = delete;

	GameConfig()
	{
		
	}
	
};