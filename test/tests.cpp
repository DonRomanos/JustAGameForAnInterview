#include "input_provider.hpp"
#include "simulation.hpp"
#include "triggers.hpp"
#include "utility.hpp"

#include "gtest/gtest.h"

using namespace std::chrono_literals;
using namespace triggers;
using namespace core;
using namespace utility;

TEST(Triggers, Input_triggers_should_execute_on_correct_phrase)
{
    //auto source = std::stringstream{};
    //auto to_test = triggers::InputTrigger{ "MyPhrase", source};
    //auto status = core::GameState{};

    //source << "Not the right Phrase" << std::endl;
    //EXPECT_FALSE(to_test.executes(status));

    //source << "MyPhrase" << std::endl;
    //EXPECT_TRUE(to_test.executes(status));
}

TEST(Triggers, Trigger_system_should_only_generate_events_from_triggers_whose_conditions_are_met)
{
    using namespace std::literals;

    auto trigger_system = EventFactory();
    auto state = core::GameState();

    trigger_system.add_trigger(Every(1000ms).aslong_as(is_alive(EntityNames::Orc)).generate_event(DealDamageEvent{.source = EntityNames::Orc, .target = EntityNames::Player, .damage = 2}));
    trigger_system.add_trigger(Every(1000ms).generate_event(DealDamageEvent{ .source = EntityNames::Dragon, .target = EntityNames::Player, .damage = 3 }));
    //trigger_system.add_trigger(OnPhrase("Attack Orc"sv).aslong_as(game_is_running).generate_event(DealDamageEvent{ .source = EntityNames::Player, .target = EntityNames::Orc, .damage = 2 }));
    //trigger_system.add_trigger(Every(1000ms).aslong_as(all_of(is_alive(EntityNames::Orc), game_is_running)).generate_event(DealDamageEvent{ .source = EntityNames::Orc, .target = EntityNames::Player, .damage = 2 }));

    trigger_system.execute(state, 1000ms);

    EXPECT_FALSE(state.current_events.empty());
}

// Had some Problems with the conan cmake_find_package generator, 
// it does not generate the correct targets therefore we dont have a gtest_main target
// so I write my own main.
int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

