#include "simulation.hpp"

using namespace simulation;

#include <algorithm>
#include <numeric>

namespace
{
	void produce_creature_events(std::vector<core::Creature>& creatures, std::vector<core::DamageEvent>& events, std::chrono::milliseconds elapsed_time)
	{
		for (auto& creature : creatures)
		{
			if( creature.health <= 0)
			{
				continue;
			}

			if (creature.current_cooldown < elapsed_time)
			{
				creature.current_cooldown = creature.attack_cooldown;
				events.emplace_back(core::DamageEvent{ .target = "player", .damage = creature.damage });
			}
			else
			{
				creature.current_cooldown = creature.current_cooldown - elapsed_time;
			}
		}
	}

	void damage_creature(const std::string& target_name, int damage, std::vector<core::Creature>& creatures)
	{
		auto target = std::find_if(std::begin(creatures), std::end(creatures), [&target_name](const auto& creature) {return creature.name == target_name; });
		if (target != std::end(creatures))
		{
			target->health -= damage;
		}
	}

	void handle_damage_events(core::Gamestate& current_state, std::vector<core::DamageEvent>& events)
	{
		std::for_each(std::begin(events), std::end(events), [&current_state](const core::DamageEvent& event)
			{
				if (event.target == "player")
				{
					current_state.player.health -= event.damage;
				}
				else
				{
					damage_creature(event.target, event.damage, current_state.creatures);
				}
			}
		);
	}

	void update_game_status(core::Gamestate& current_state)
	{
		if (current_state.player.health <= 0)
		{
			current_state.status = core::GameResult::Lost;
			return;
		}

		const int total_creature_health = std::accumulate(std::begin(current_state.creatures), std::end(current_state.creatures), 0, 
			[](int sum, const auto& creature) { return sum + creature.health; });
		if ( total_creature_health <= 0)
		{
			current_state.status = core::GameResult::Won;
		}
		
	}
}

simulation::Engine::Engine(std::chrono::steady_clock::time_point init_time) : last_execution(init_time)
{
}

void simulation::Engine::advance_gamestate(core::Gamestate& current_state, std::vector<core::DamageEvent>& events)
{
	update_game_status(current_state);

	if (current_state.status != core::GameResult::Running)
	{
		return;
	}

	auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - last_execution);

	produce_creature_events(current_state.creatures, events, elapsed_time);

	handle_damage_events(current_state, events);

	
}
