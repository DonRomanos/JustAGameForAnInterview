#include "gtest/gtest.h"

#include "console_input.hpp"

#include <strstream>

TEST(Input, Input_should_react_only_after_pressing_enter)
{
    using namespace std::chrono_literals;

    auto sample_stream = std::stringstream();
    auto to_test = input::Console(sample_stream);

    sample_stream << "NotYet";
    EXPECT_TRUE(to_test.poll_keyboard().empty());

    sample_stream << "Enter" << std::endl;
    std::this_thread::sleep_for(10ms);
    EXPECT_EQ(to_test.poll_keyboard(), "NotYetEnter");
}

TEST(GtestDependency, Should_Compile_if_gtest_was_found)
{
    SUCCEED();
}

// Had some Problems with the conan cmake_find_package generator, 
// it does not generate the correct targets therefore we dont have a gtest_main target
// so I write my own main.
int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

