#pragma once

#include <string>

namespace input
{
	class Provider
	{
	public:
		[[nodiscard]] virtual std::string poll_keyboard() = 0;
	};
}