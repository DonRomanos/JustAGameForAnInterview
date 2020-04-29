#pragma once

#include <string>
#include <chrono>
#include <vector>

namespace simulation
{
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

	struct Gamestate
	{
		std::vector<Creature> creatures;
		Player player;
	};


	class Engine
	{
	public:
		void AdvanceGamestate(Gamestate&);
	
	private:
		std::chrono::steady_clock::time_point last_execution;
	};
}