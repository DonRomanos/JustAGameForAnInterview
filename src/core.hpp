#pragma once

#include <string>
#include <vector>
#include <chrono>

namespace core
{
	enum class GameResult
	{
		Running,
		Lost,
		Won
	};

	struct Creature
	{
		const std::string name;
		const std::chrono::milliseconds attack_cooldown;
		const int damage;

		int health;
		std::chrono::milliseconds current_cooldown;
	};

	struct Player
	{
		int health;
		std::string current_input;
	};

	struct DamageEvent
	{
		std::string source;
		std::string target;
		int damage;
	};

	struct Gamestate
	{
		std::vector<Creature> creatures;
		Player player;
		GameResult status = GameResult::Running;
	};
}