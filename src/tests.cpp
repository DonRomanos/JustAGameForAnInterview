#include "input_provider.hpp"
#include "simulation.hpp"

#include "gtest/gtest.h"

#include <sstream>

using namespace std::chrono_literals;

// These tests could be better written using Catch or boost/unittest once its ready...
TEST(Simulation, Attack_of_the_player_deals_damage_to_correct_creature)
{
    auto state = core::Gamestate{
        .creatures = {core::Creature{.name = "Orc", .damage = 0, .health = 42, .current_cooldown = 1000ms}},
        .player{.health = 10} };
    auto events = std::vector<core::DamageEvent>{ core::DamageEvent{.target = "Orc", .damage = 42 } };
    auto engine = simulation::Engine{};

    engine.advance_gamestate(state, events);

    ASSERT_EQ(state.creatures.front().health, 0);
}

TEST(Simulation, Dead_Monsters_should_not_deal_damage)
{
    auto state = core::Gamestate{ 
        .creatures = {
            core::Creature{.name = "Orc", .damage = 1000, .health = 0, .current_cooldown = 0ms},
            core::Creature{.name = "Orc2", .damage = 1000, .health = 1, .current_cooldown = 1000ms}},
        .player{.health = 42}};
    auto events = std::vector<core::DamageEvent>{};
    auto engine = simulation::Engine{};

    engine.advance_gamestate(state, events);

    EXPECT_TRUE(events.empty());
    EXPECT_EQ(state.player.health, 42);
}

TEST(Simulation, Losing_the_last_healthpoint_should_lose_the_game)
{
    auto state = core::Gamestate{
        .creatures = {core::Creature{.name = "Orc", .damage = 0, .health = 42, .current_cooldown = 1000ms}},
        .player{.health = 0} };
    auto events = std::vector<core::DamageEvent>{};
    auto engine = simulation::Engine{};

    engine.advance_gamestate(state, events);

    EXPECT_EQ(state.status, core::GameResult::Lost);
}

TEST(Simulation, Killing_all_the_monsters_should_win_the_game)
{
    auto state = core::Gamestate{
        .creatures = {core::Creature{.name = "Orc", .damage = 0, .health = 0, .current_cooldown = 1000ms}},
        .player{.health = 10} };
    auto events = std::vector<core::DamageEvent>{};
    auto engine = simulation::Engine{};
    
    engine.advance_gamestate(state, events);

    EXPECT_EQ(state.status, core::GameResult::Won);
}

TEST(Simulation, Creatures_attack_after_specified_time)
{
    auto state = core::Gamestate{
        .creatures = {core::Creature{.name = "Orc", .damage = 9, .health = 10, .current_cooldown = 10ms}},
        .player{.health = 10} };
    auto events = std::vector<core::DamageEvent>{};
    auto engine = simulation::Engine{std::chrono::steady_clock::now() - 1000ms};

    engine.advance_gamestate(state, events);

    EXPECT_EQ(events.size(), 1);
    EXPECT_EQ(state.player.health, 1);
}

TEST(Input, Typing_Attack_Orc_should_produce_correct_event)
{
    using namespace std::chrono_literals;

    auto sample_stream = std::stringstream();
    auto to_test = input::Provider(sample_stream);

    sample_stream << "Attack ";
    EXPECT_FALSE(to_test.poll_keyboard().has_value());

    sample_stream << "Orc" << std::endl;
    std::this_thread::sleep_for(10ms);
    EXPECT_EQ(to_test.poll_keyboard()->target, "Orc");
}

// Had some Problems with the conan cmake_find_package generator, 
// it does not generate the correct targets therefore we dont have a gtest_main target
// so I write my own main.
int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

