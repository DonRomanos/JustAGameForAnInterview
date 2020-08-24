#pragma once

#include "system.hpp"

#include <ostream>

namespace output
{
	class Renderer : public core::System
	{
	public:
		Renderer(std::ostream& sink);

		virtual void execute(core::GameState& current_state) override;

	private:
		std::ostream& sink;
		std::string input_rendered_so_far; // Have to keep track since the terminal itself remembers
	};
}