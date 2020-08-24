#pragma once

#include "core.hpp"

namespace utility
{
	// Some helpers for lambda visitors
	template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
	template<class... Ts> overloaded(Ts...)->overloaded<Ts...>; // should not needed as of C++20 but for me it still is in VS2019


	/*****************************************************************/
	[[nodiscard]] constexpr auto is_alive(const core::EntityNames name)
	{
		return [name](const core::GameState& state) noexcept { return state.entities[static_cast<int>(name)].health > 0; };
	}

	[[nodiscard]] constexpr bool game_is_running(const core::GameState& state) noexcept
	{
		return state.shape == core::GameShape::Running;
	}

	[[nodiscard]] core::GameState create_default_state();
}