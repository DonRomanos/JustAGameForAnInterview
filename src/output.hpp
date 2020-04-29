#pragma once

#include "core.hpp"

#include <ostream>

namespace output
{
	class Printer
	{
	public:
		explicit Printer(std::ostream& out);

		void produce_output(const core::Gamestate& state, const std::vector<core::DamageEvent>& events);

	private:
		std::ostream& sink;
	};
}