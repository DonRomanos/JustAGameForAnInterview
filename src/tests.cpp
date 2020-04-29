#include "console_input.hpp"

#include "gtest/gtest.h"

#include <strstream>

TEST(Simulation, Attack_of_the_player_deals_damage_to_correct_creature)
{
    FAIL();
}

TEST(Simulation, Losing_the_last_healthpoint_should_lose_the_game)
{
    FAIL();
}

TEST(Simulation, Killing_all_the_monsters_should_win_the_game)
{
    FAIL();
}

TEST(Simulation, Creatures_attack_after_specified_time)
{
    FAIL();
}

TEST(Simulation, Damage_dealt_is_subtracted_from_health)
{
    FAIL();
}

TEST(Input, Input_should_react_only_after_pressing_enter)
{
    using namespace std::chrono_literals;

    auto sample_stream = std::stringstream();
    auto to_test = input::AsyncProvider(sample_stream);

    sample_stream << "NotYet";
    EXPECT_TRUE(to_test.poll_keyboard().empty());

    sample_stream << "Enter" << std::endl;
    std::this_thread::sleep_for(10ms);
    EXPECT_EQ(to_test.poll_keyboard(), "NotYetEnter");
}

// Had some Problems with the conan cmake_find_package generator, 
// it does not generate the correct targets therefore we dont have a gtest_main target
// so I write my own main.
int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

