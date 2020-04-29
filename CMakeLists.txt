cmake_minimum_required(VERSION 3.13)

project(JustAGameForAnInterview)

option(BUILD_TESTS "Will build the tests verifying your result" ON)

add_executable(${PROJECT_NAME})
add_executable(Challenge::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PRIVATE src/)

target_sources(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/console_input.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/simulation.cpp)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
        -Wall -Werror -pedantic-errors -Wextra -Wconversion -Wsign-conversion>
    $<$<CXX_COMPILER_ID:MSVC>:
        /WX /W4 /std:c++latest>
)

######################################################################
if( BUILD_TESTS)
    find_package(gtest REQUIRED)

    add_executable(${PROJECT_NAME}_Tests)
    
    target_sources(${PROJECT_NAME}_Tests PRIVATE 
        src/tests.cpp
        src/console_input.cpp
        src/simulation.cpp
        src/output.cpp
    )

    target_include_directories(${PROJECT_NAME}_Tests PRIVATE
        src/)

    target_link_libraries(${PROJECT_NAME}_Tests PRIVATE
        gtest::gtest
    )

    target_compile_options(${PROJECT_NAME}_Tests PRIVATE
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
            -Wall -Werror -pedantic-errors -Wextra -Wconversion -Wsign-conversion>
        $<$<CXX_COMPILER_ID:MSVC>:
            /WX /W4 /std:c++latest>
    )

    include(GoogleTest)
    enable_testing()
    gtest_discover_tests(${PROJECT_NAME}_Tests)
endif()