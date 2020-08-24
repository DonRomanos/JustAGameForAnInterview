#pragma once

#include "core.hpp"
#include "system.hpp"

#include <string>
#include <chrono>
#include <vector>

namespace simulation
{
	class EventHandler : public core::System
	{
	public:
		virtual void execute(core::GameState& current_state) override;
	};
}