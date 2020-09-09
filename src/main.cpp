#include "input_provider.hpp"
#include "simulation.hpp"
#include "output.hpp"
#include "utility.hpp"
#include "triggers.hpp"

#include <iostream>
#include <memory>
#include <string>

namespace
{
    [[nodiscard]] std::chrono::milliseconds time_since_last_update() noexcept
    {
        static auto since_last = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - since_last);
        since_last = std::chrono::steady_clock::now();
        return elapsed;
    }

    void prepare_for_next_iteration(core::GameState& game) noexcept
    {
        std::swap(game.current_events, game.future_events);
        game.future_events.clear();
        std::swap(game.shape, game.future_shape);
        game.future_shape = core::GameShape::Running;
    }
}

int main(/*int argc, char** argv*/)
{
    using namespace std::chrono_literals;
    using namespace triggers;
    using namespace utility;
    using namespace core;

    auto input_provider = input::KeyboardInput();
    auto trigger_system = triggers::EventFactory();
    auto game_logic     = simulation::EventHandler();
    auto renderer = output::Renderer(std::cout);

    trigger_system.add_trigger(Every(1400ms).aslong_as(is_alive(EntityNames::Orc)).generate_event(DealDamageEvent{ .source = EntityNames::Orc, .target = EntityNames::Player, .damage = 2 }));
    trigger_system.add_trigger(Every(2200ms).aslong_as(is_alive(EntityNames::Dragon)).generate_event(DealDamageEvent{ .source = EntityNames::Dragon, .target = EntityNames::Player, .damage = 3 }));
    
    // TODO fix triggers that you can damage dead entities.
    trigger_system.add_trigger(OnPhrase("attack orc", input_provider).aslong_as(is_alive(EntityNames::Player)).generate_event(DealDamageEvent{ .source = EntityNames::Player, .target = EntityNames::Orc, .damage = 2 }));
    trigger_system.add_trigger(OnPhrase("attack dragon", input_provider).aslong_as(is_alive(EntityNames::Player)).generate_event(DealDamageEvent{ .source = EntityNames::Player, .target = EntityNames::Dragon, .damage = 2 }));


    core::GameState game = utility::create_default_state();
    auto elapsed_time = time_since_last_update();
    while (true)
    {
        elapsed_time = time_since_last_update();
        game.current_input += input_provider.query_pending_input();

        trigger_system.execute(game, elapsed_time);
        game_logic.execute(game);
        renderer.execute(game);

        prepare_for_next_iteration(game);

        std::this_thread::sleep_for(10ms);
    };

    std::cin.get();

    return 0;
}