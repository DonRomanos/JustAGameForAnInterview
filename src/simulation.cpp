#include "simulation.hpp"
#include "utility.hpp"

#include <algorithm>
#include <numeric>

using namespace simulation;

namespace
{
	[[nodiscard]] constexpr bool areAllMonstersDead(const core::GameState& state) noexcept
	{
		return !(utility::is_alive(core::EntityNames::Orc)(state) ||
			utility::is_alive(core::EntityNames::Dragon)(state));
	}

	[[nodiscard]] constexpr bool isPlayerDead(const core::GameState& state) noexcept
	{
		return !utility::is_alive(core::EntityNames::Player)(state);
	}

	[[nodiscard]] auto event_handler(core::GameState& state)
	{
		return utility::overloaded{
			[&state](const core::DealDamageEvent& event) {
				state.entities[static_cast<int>(event.target)].health -= event.damage;
				if (areAllMonstersDead(state)) { state.current_events.push_back(core::GameShapeChangeEvent{ .new_shape = core::GameShape::Won }); }
				else if(isPlayerDead(state)) { state.current_events.push_back(core::GameShapeChangeEvent{ .new_shape = core::GameShape::Lost }); };
				},
			[&state](const core::GameShapeChangeEvent& event) {
			if (event.new_shape == core::GameShape::Restart) state = utility::create_default_state(); } 
			};
	}
}

void simulation::EventHandler::execute(core::GameState& current_state)
{
	for (const auto& event : current_state.current_events)
	{
		std::visit(event_handler(current_state), event);
	}
}
