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

	struct HandleEvents
	{
		HandleEvents(core::GameState& game) : game(game) {};

		[[nodiscard]] constexpr void operator()(const core::DealDamageEvent& damageEvent) noexcept
		{
			if (game.entities[damageEvent.target].health > damageEvent.damage)
			{
				game.entities[damageEvent.target].health -= damageEvent.damage;
			}
			else if(game.entities[damageEvent.target].health > 0) // prevents "double" killing if events happen synchronously
			{
				game.entities[damageEvent.target].health = 0;
				game.future_events.emplace_back(core::KillEvent{ .killer{damageEvent.source}, .victim{damageEvent.target} });
			}
		}

		[[nodiscard]] constexpr void operator()(const core::KillEvent& event) noexcept
		{
			if (event.victim == core::EntityNames::Player)
			{
				game.future_shape = core::GameShape::Lost;
			}
			else if (areAllMonstersDead(game))
			{
				game.future_shape = core::GameShape::Won; // Player loses even if he kills all monsters simultaneously.
			}
		}

		core::GameState& game;
	};

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

void simulation::EventHandler::execute(core::GameState& game)
{
	advance_game_state(game);

	for (const auto& event : game.current_events)
	{
		std::visit(HandleEvents{ game }, event);
	}
}
