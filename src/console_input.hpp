#include "input.hpp"

#include <future>

namespace input
{
	class Console : public input::Provider
	{
	public:
		explicit Console(std::istream& source);

		virtual std::string poll_keyboard() override;

	private:
		std::istream& source;
		std::future<std::string> next_line;
	};
}