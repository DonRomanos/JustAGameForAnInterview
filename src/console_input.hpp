#include "input.hpp"

#include <future>

namespace input
{
	class AsyncProvider : public input::Provider
	{
	public:
		explicit AsyncProvider(std::istream& source);

		[[nodiscard]] virtual std::string poll_keyboard() override;

	private:
		std::istream& source;
		std::future<std::string> next_line;
	};
}