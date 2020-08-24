#pragma once

#include "core.hpp"

#include <future>
#include <optional>
#include <istream>

namespace input
{
	class ProviderI
	{
	public:
		[[nodiscard]] virtual std::string_view query_pending_input() = 0;
		virtual ~ProviderI() = default;
	};

	// Sequentally provides elements from a predefined list.
	class FixedInput : public ProviderI
	{
	public:
		FixedInput(std::initializer_list<std::string> values) : contents(values)
		{};

		std::string_view query_pending_input() override;
	private:
		size_t next_element = 0;
		std::vector<std::string> contents;
	};

	// Here the fun begins...
	class KeyboardInput : public ProviderI
	{
	public:
		std::string_view query_pending_input() override;

	private:
		std::string buffer;
	};
}