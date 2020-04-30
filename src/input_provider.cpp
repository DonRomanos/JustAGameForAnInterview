#include "input_provider.hpp"

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

input::Provider::Provider(std::istream& in) : source(in)
{
	next_line = start_reading_next_line(in);
}

namespace
{
	std::optional<core::DamageEvent> to_event(const std::string& line)
	{
		const int player_damage = 2;

		if (line.substr(0, 7) == "attack " && line.size() > 7)
		{
			return core::DamageEvent{ .source = "player", .target = line.substr(7, line.size() - 7), .damage = player_damage  };
		}

		return std::nullopt;
	}
}

std::optional<core::DamageEvent> input::Provider::poll_keyboard()
{
	using namespace std::chrono_literals;

	std::string result;
	if (next_line.wait_for(0ms) == std::future_status::ready) // there is a proposal for properly checking the state of a future but not yet available
	{
		result = next_line.get();
		next_line = start_reading_next_line(source);
	}
	return to_event(result);
}