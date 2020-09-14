#include "input_provider.hpp"

#include <conio.h>

#include <chrono>
#include <istream>
#include <thread>
#include <mutex>

namespace
{
	std::optional<unsigned char> to_char(int symbol)
	{
		// arrow keys with conio consist of 2 characters we want to ignore them both.
		static bool ignore_next = false;
		if (ignore_next)
		{
			ignore_next = false;
			return std::nullopt;
		}

		constexpr unsigned char is_arrow_key = 224;
		switch (symbol)
		{
			case '\r': return '\n';         // on windows enter is recognized as \r instead of \n
			case '\b': return std::nullopt; // dont want to allow backspace a the moment!
			case is_arrow_key:
			{
				ignore_next = true;
				return std::nullopt;
			}
		}

		constexpr int ASCII_normal_symbols_begin = 32;
		constexpr int ASCII_normal_symbols_end = 126;
		symbol = std::clamp(symbol, ASCII_normal_symbols_begin, ASCII_normal_symbols_end);

		return static_cast<unsigned char>(symbol);
	}
}

/********************************************************/
class AsynchronousInputPoller
{
public:
	AsynchronousInputPoller() : should_stop(false)
	{
		polling_keypresses = std::thread{ [this]() {
			while (!should_stop.load(std::memory_order_relaxed))
			{
				if (_kbhit() != 0)
				{
					std::scoped_lock<std::mutex> lock{ buffer_lock };
					if( auto x = to_char(_getch()))
						buffer += *x; 
				}
			}
		} };
	}

	std::string transfer_from_buffer()
	{
		std::scoped_lock<std::mutex> lock{buffer_lock};
		auto temp = buffer;
		buffer.clear();
		return temp;
	}

	~AsynchronousInputPoller()
	{
		should_stop.store(true, std::memory_order_relaxed);
		polling_keypresses.join();
	}
private:
	std::atomic<bool> should_stop;
	std::thread polling_keypresses;
	std::mutex buffer_lock;
	std::string buffer;
};

std::string_view input::KeyboardInput::query_pending_input()
{
	// If you instantiate more than one of this its your own problem!
	static AsynchronousInputPoller source;
	buffer = source.transfer_from_buffer();
	return buffer;
}

std::string_view input::FixedInput::query_pending_input()
{
	if (next_element >= contents.size())
	{
		throw std::runtime_error("Exceeding contents of FixedInput Provider.");
	}

	return contents[next_element++];
}
