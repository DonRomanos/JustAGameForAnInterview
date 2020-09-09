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

	[[nodiscard]] constexpr auto handle_damage_event(core::GameState& state) noexcept
	{
		return [&state](const core::DealDamageEvent& damageEvent)
		{
			state.entities[damageEvent.target].health -= damageEvent.damage;
			if (areAllMonstersDead(state))
			{
				state.future_shape = core::GameShape::Won; // Player wins even if he dies simultaneously 
			}
			else if (isPlayerDead(state))
			{
				state.future_shape = core::GameShape::Lost;
			};
		};
	}

	constexpr void advance_game_state(core::GameState& state) noexcept
	{
		switch (state.shape)
		{
		case core::GameShape::Won:
			[[fallthrough]];
		case core::GameShape::Lost:
			state.future_shape = core::GameShape::Restart;
			break;
		case core::GameShape::Restart:
			state = utility::create_default_state();
			state.future_shape = core::GameShape::Running;
			break;
		case core::GameShape::Running:
			break;
		}
	}

}

void simulation::EventHandler::execute(core::GameState& current_state)
{
	advance_game_state(current_state);

	for (const auto& event : current_state.current_events)
	{
		std::visit(handle_damage_event(current_state), event); 
	}
}
