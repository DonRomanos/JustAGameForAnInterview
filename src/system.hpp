#pragma once

#include "core.hpp"

#include <chrono>

namespace core
{
	class TimedSystem
	{
	public:
		virtual void execute(core::GameState& current_state, const std::chrono::milliseconds elapsed_time) = 0;
		virtual ~TimedSystem() = 0 {};
	};

	class System
	{
	public:
		virtual void execute(core::GameState& current_state) = 0;
		virtual ~System() = 0 {};
	};
}