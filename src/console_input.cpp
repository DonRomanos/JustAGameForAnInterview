#include "console_input.hpp"

#include <chrono>
#include <istream>

namespace
{
	std::future<std::string> start_reading_next_line(std::istream& in)
	{
		return std::async(std::launch::async, [&in]()
			{
				std::string result;
				std::getline(in, result);
				return result;
			}
		);
	}
}

input::AsyncProvider::AsyncProvider(std::istream& in) : source(in)
{
	next_line = start_reading_next_line(in);
}

std::string input::AsyncProvider::poll_keyboard()
{
	using namespace std::chrono_literals;

	std::string result;
	if (next_line.wait_for(0ms) == std::future_status::ready) // there is a proposal for properly checking the state of a future but not yet available
	{
		result = next_line.get();
		next_line = start_reading_next_line(source);
	}
	return result;
}
