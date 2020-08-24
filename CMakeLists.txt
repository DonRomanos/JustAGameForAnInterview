cmake_minimum_required(VERSION 3.13)

project(JustAGameForAnInterview)

option(BUILD_TESTS "Will build the tests verifying your result" ON)

add_library(engine)
add_library(core::engine ALIAS engine)

target_include_directories(engine PUBLIC src/)

target_sources(engine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/input_provider.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/simulation.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/output.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/triggers.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utility.cpp
)

target_compile_features(engine PUBLIC cxx_std_20)

target_compile_options(engine PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
        -Wall -Werror -pedantic-errors -Wextra -Wconversion -Wsign-conversion -std=c++2a>
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:
        -Wno-c99-extensions>
    $<$<CXX_COMPILER_ID:MSVC>:
        /WX /W4 /std:c++latest>
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

target_link_libraries(engine PRIVATE 
    Threads::Threads
)

######################################################################
add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE
    src/main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    core::engine)

######################################################################
if( BUILD_TESTS)
    find_package(gtest REQUIRED)

    add_executable(${PROJECT_NAME}_Tests)
    
    target_sources(${PROJECT_NAME}_Tests PRIVATE 
        test/tests.cpp
        test/input_tests.cpp
    )

    target_link_libraries(${PROJECT_NAME}_Tests PRIVATE
        gtest::gtest
        core::engine
    )

    target_compile_options(${PROJECT_NAME}_Tests PRIVATE
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
            -Wall -Werror -pedantic-errors -Wextra -Wconversion -Wsign-conversion -std=c++2a -Wno-missing-field-initializers>
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:
            -Wno-c99-extensions>
        $<$<CXX_COMPILER_ID:MSVC>:
            /WX /W4 /std:c++latest>
    )

    include(GoogleTest)
    enable_testing()
    gtest_discover_tests(${PROJECT_NAME}_Tests)
endif()