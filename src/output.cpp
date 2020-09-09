#include "output.hpp"
#include "utility.hpp"

#include <string_view>
#include <optional>

namespace
{
	[[nodiscard]] std::string starting_text()
	{
		using namespace std::literals;

		return "Starting Game ...\n"s +
		       "Type and press enter to attack:\n"s +
			   "\tattack orc\n"s +
		       "\tattack dragon\n\n"s +
		       std::string(80, '*') + "\n";
	}

	[[nodiscard]] std::string congratulations()
	{
		using namespace std::literals;

		return "\n"s + std::string(80, '*') + 
			   "\nCongratulations! You won! Woooho\n"s +
			   std::string(80, '*') + "\n";
	}

	[[nodiscard]] std::string shame()
	{
		using namespace std::literals;

		return "\n"s + std::string(80, '*') +
			"\nShame! You lost! Try again\n"s +
			std::string(80, '*') + "\n";
	}

	[[nodiscard]] std::string render_damage(const core::EntityContainer& entities, const core::DealDamageEvent& event)
	{
		if (entities[event.target].health <= 0)
		{
			return entities[event.source].name + " killed " + entities[event.target].name;
		}
		else
		{
			return entities[event.source].name + " damaged " + entities[event.target].name
				+ " for " + std::to_string(event.damage) + " health";
		}
	}

	[[nodiscard]] std::string render_gamestate(const core::GameShape state)
	{
		switch (state)
		{
			case core::GameShape::Restart: return starting_text();
			case core::GameShape::Won: return congratulations();
			case core::GameShape::Lost: return shame();
			case core::GameShape::Running: [[fallthrough]];
			default: return "";
		}
	}
}

output::Renderer::Renderer(std::ostream& sink) : sink(sink), input_rendered_so_far()
{
}

void output::Renderer::execute(core::GameState& game)
{
	using namespace std::literals;

	std::string to_render_from_system = "";
	for (const auto& event : game.current_events)
	{
		to_render_from_system += std::visit([&entities = game.entities](const core::DealDamageEvent& event)
			{ return render_damage(entities, event); }, event) + "\n"s;
	}

	to_render_from_system = render_gamestate(game.shape);

	if (!to_render_from_system.empty())
	{
		input_rendered_so_far.clear();
		sink << "\r" << std::string(100, ' ') << '\r' << to_render_from_system;
	}

	sink << game.current_input.substr(input_rendered_so_far.size(), game.current_input.size() - input_rendered_so_far.size());
	auto next_linebreak = game.current_input.find('\n');
	if (next_linebreak == std::string::npos)
	{
		input_rendered_so_far = game.current_input;
	}
	else
	{
		input_rendered_so_far = game.current_input.substr(next_linebreak + 1, game.current_input.size() - next_linebreak - 1);
		game.current_input.erase(0, next_linebreak + 1);
	}
}
