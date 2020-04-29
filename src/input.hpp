#pragma once

#include <string>

namespace input
{
	class Provider
	{
	public:
		virtual std::string poll_keyboard() = 0;
	};
}