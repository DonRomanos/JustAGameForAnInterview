#pragma once

#include "core.hpp"

#include <future>
#include <optional>
#include <istream>

namespace input
{
	class Provider
	{
	public:
		explicit Provider(std::istream& source);

		[[nodiscard]] std::optional<core::DamageEvent> poll_keyboard();

	private:
		std::istream& source;
		std::future<std::string> next_line;
	};
}