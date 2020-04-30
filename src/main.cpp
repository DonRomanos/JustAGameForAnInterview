
#include "input_provider.hpp"
#include "simulation.hpp"
#include "output.hpp"

#include <iostream>
#include <memory>
#include <string>

namespace
{
    void display_victory_defeat_message(core::GameResult result)
    {
        if (result == core::GameResult::Lost)
        {
            std::cout << "\n\nDefeat! - Press enter twice to quit." << std::endl;
        }
        else
        {
            std::cout << "\n\nVictory! - Press enter twice to quit." << std::endl;
        }
    }
}

int main(/*int argc, char** argv*/)
{
    using namespace std::chrono_literals;

    std::cout << "Starting Game ...\n";
    std::cout << "Type and press enter to attack:\n";
    std::cout << "\tattack orc\n";
    std::cout << "\tattack dragon\n\n";
    std::cout << std::string(80, '*') << std::endl;

    core::Gamestate game_state;
    game_state.creatures.emplace_back(core::Creature{ .name = "orc", .attack_cooldown = 1400ms, .damage = 1, .health = 7, .current_cooldown = 1400ms });
    game_state.creatures.emplace_back(core::Creature{ .name = "dragon", .attack_cooldown = 2200ms, .damage = 3, .health = 20, .current_cooldown = 2200ms });
    game_state.player.health = 40;
    game_state.status = core::GameResult::Running;

    auto input_provider = input::Provider(std::cin);
    auto engine = simulation::Engine();
    auto output = output::Printer(std::cout);

    std::vector<core::DamageEvent> events;
    while (game_state.status == core::GameResult::Running)
    {
        events.clear();
        auto input = input_provider.poll_keyboard();
        if (input)
        {
            events.push_back(*input);
        }
        engine.advance_gamestate(game_state, events);
        output.produce_output(game_state, events);
        std::this_thread::sleep_for(10ms); // If I don#t sleep here millisecond resolution is not enough for the timer or my computer is too fast...
    };

    display_victory_defeat_message(game_state.status);
    std::cin.get();

    return 0;
}