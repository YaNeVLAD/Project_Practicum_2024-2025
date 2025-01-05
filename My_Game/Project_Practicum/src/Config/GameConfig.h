#pragma once

#include <string>
#include <fstream>
#include <unordered_map>
#include <stdexcept>

class GameConfig
{
public:
	enum class PlayerType
	{
		First,
		Second,
	};

	const char* Axe = "Axe";
	const char* MagicCharge = "Magic Charge";
	const char* Lightning = "Lightning Strike";
	const char* Fireball = "Fireball";
	const char* Book = "Book";

	static GameConfig* GetInstance()
	{
		if (mInstance == nullptr)
		{
			mInstance = new GameConfig();
		}
		return mInstance;
	}

	size_t upgradePoints = 0;
	size_t playerHealthBuff = 0;
	size_t playerDamageBuff = 0;
	size_t killedEnemies = 0;
	size_t killedBosses = 0;
	size_t maxBosses = 1;
	PlayerType playerType = PlayerType::First;
	bool isInfinite = false;
	std::unordered_map<std::string, int> weaponStats =
	{ {Axe, 0}, {MagicCharge, 0}, {Lightning, 0}, {Fireball, 0}, {Book, 0} };

	void SaveConfig(const std::string& filePath = "common/stats.conf")
	{
		std::ofstream file(filePath, std::ios::binary);
		if (!file.is_open()) throw std::runtime_error("Failed to open file for saving");

		WriteValue(file, playerDamageBuff);
		WriteValue(file, playerHealthBuff);
		WriteValue(file, upgradePoints);

		size_t mapSize = weaponStats.size();
		WriteValue(file, mapSize);
		for (const auto& [key, value] : weaponStats)
		{
			WriteString(file, key);
			WriteValue(file, value);
		}
	}

	void LoadConfig(const std::string& filePath = "common/stats.conf")
	{
		std::ifstream file(filePath, std::ios::binary);
		if (!file.is_open()) return;

		try
		{
			ReadValue(file, playerDamageBuff);
			ReadValue(file, playerHealthBuff);
			ReadValue(file, upgradePoints);

			size_t mapSize;
			ReadValue(file, mapSize);
			weaponStats.clear();
			for (size_t i = 0; i < mapSize; ++i)
			{
				std::string key;
				ReadString(file, key);
				int value;
				ReadValue(file, value);
				weaponStats[key] = value;
			}
		}
		catch (...)
		{
			ResetToDefaults();
		}
	}

	void ResetPoints()
	{
		int weaponUpgrades = 0;
		for (auto& [weapon, upgrades] : weaponStats)
		{
			weaponUpgrades += upgrades;
			upgrades = 0;
		}

		upgradePoints = upgradePoints + playerDamageBuff + playerHealthBuff + weaponUpgrades;

		playerDamageBuff = 0;
		playerHealthBuff = 0;
	}

private:
	static GameConfig* mInstance;

	GameConfig(const GameConfig&) = delete;
	GameConfig operator=(const GameConfig&) = delete;

	GameConfig() {}

	template <typename T>
	void WriteValue(std::ofstream& file, const T& value)
	{
		file.write(reinterpret_cast<const char*>(&value), sizeof(T));
	}

	void WriteString(std::ofstream& file, const std::string& value)
	{
		size_t length = value.size();
		WriteValue(file, length);
		file.write(value.data(), length);
	}

	template <typename T>
	void ReadValue(std::ifstream& file, T& value)
	{
		file.read(reinterpret_cast<char*>(&value), sizeof(T));
		if (file.fail()) throw std::runtime_error("Failed to read value");
	}

	void ReadString(std::ifstream& file, std::string& value)
	{
		size_t length;
		ReadValue(file, length);
		value.resize(length);
		file.read(&value[0], length);
		if (file.fail()) throw std::runtime_error("Failed to read string");
	}

	void ResetToDefaults()
	{
		upgradePoints = 0;
		playerDamageBuff = 0;
		playerHealthBuff = 0;
		killedEnemies = 0;
		killedBosses = 0;
		maxBosses = 1;
		playerType = PlayerType::First;
		isInfinite = false;
		weaponStats.clear();
	}
};