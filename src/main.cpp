
#include "console_input.hpp"

#include <iostream>
#include <memory>
#include <string>

int main(/*int argc, char** argv*/)
{
    using namespace std::chrono_literals;

    auto input_provider = std::make_unique<input::Console>(std::cin);


    std::cout << "Starting Game ..." << std::endl;

    while(true)
    {
        std::cout << "next" << std::endl;
        std::string line = input_provider->poll_keyboard();
        if (!line.empty())
        {
            std::cout << "input: " << line << std::endl;
        }
        std::this_thread::sleep_for(100ms);
    };

    return 0;
}