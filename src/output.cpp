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
		if (entities[static_cast<int>(event.target)].health <= 0)
		{
			return entities[static_cast<int>(event.source)].name + " killed " + entities[static_cast<int>(event.target)].name;
		}
		else
		{
			return entities[static_cast<int>(event.source)].name + " damaged " + entities[static_cast<int>(event.target)].name
				+ " for " + std::to_string(event.damage) + " health";
		}
	}

	[[nodiscard]] std::string render_gamestate(const core::GameShapeChangeEvent event)
	{
		switch (event.new_shape)
		{
			case core::GameShape::Restart: return starting_text();
			case core::GameShape::Won: return congratulations();
			case core::GameShape::Lost: return shame();
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

	std::string event_output = "";
	for (const auto& event : game.current_events)
	{
		event_output += std::visit(utility::overloaded{
			[&entities = game.entities] (const core::DealDamageEvent& event){ return render_damage(entities, event); },
			[](const core::GameShapeChangeEvent event) {return render_gamestate(event); }
			}, event) + "\n"s;
	}

	
	if (!event_output.empty())
	{
		input_rendered_so_far.clear();
		sink << "\r" << std::string(100, ' ') << '\r' << event_output;
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
