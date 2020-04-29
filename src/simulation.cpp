#include "simulation.hpp"

using namespace simulation;

namespace
{
	std::vector<Creature> UpdateMonsters(const std::vector<Creature>& creatures, std::chrono::milliseconds elapsed_time)
	{
		creatures;
		elapsed_time;
		return {};
	}
}

Gamestate simulation::Engine::AdvanceSimulation(const Gamestate&)
{
	auto elapsed_time = std::chrono::steady_clock::now() - last_execution;



	return Gamestate();
}
