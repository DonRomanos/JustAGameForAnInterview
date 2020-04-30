#include "output.hpp"

#include <string_view>
#include <optional>

output::Printer::Printer(std::ostream& out) : sink(out)
{
}

namespace
{
	std::optional<int> target_health(const std::string& target_name, const core::Gamestate& state)
	{
		if (target_name == "player")
		{
			return state.player.health;
		}
		else
		{
			if (auto target = std::find_if(std::begin(state.creatures), std::end(state.creatures), [target_name](const auto& creature) {return creature.name == target_name; });
				target != std::end(state.creatures))
			{
				return target->health;
			}
		}
		return std::nullopt;
	}
}

void output::Printer::produce_output(const core::Gamestate& state, const std::vector<core::DamageEvent>& events)
{
	for (const auto& event : events)
	{
		auto health = target_health(event.target, state);
		if (!health)
		{
			continue;
		}
		auto message = event.source + " attacks: " + event.target + " for " + std::to_string(event.damage) + " damage! "; // std::format would be cool here.
		health <= 0 ? message += (event.target + " died.") : message += (event.target + " has now " + std::to_string(*health) + " health.");
		sink << message << std::endl;
	}
}
