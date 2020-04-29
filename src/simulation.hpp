#pragma once

#include "core.hpp"

#include <string>
#include <chrono>
#include <vector>

namespace simulation
{
	class Engine
	{
	public:
		explicit Engine(std::chrono::steady_clock::time_point init_time = std::chrono::steady_clock::now());

		void advance_gamestate(core::Gamestate& state, std::vector<core::DamageEvent>& events);
	
	private:
		std::chrono::steady_clock::time_point last_execution;
	};
}