cmake_minimum_required(VERSION 3.13)

project(Challenge)

option(BUILD_TESTS "Will build the tests verifying your result" ON)
option(BUILD_REFERENCE "Will build a reference implementation as a library, 
                        which will also be tested and benchmarked. Cool for comparison." ON)
option(BUILD_BENCHMARK "This will enable the performance tests using google benchmark" ON)

add_library(${PROJECT_NAME} INTERFACE)
add_library(Challenge::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} INTERFACE src/)

target_sources(${PROJECT_NAME} INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/challenge.hpp)

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_20)

target_compile_options(${PROJECT_NAME} INTERFACE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
        -Wall -Werror -pedantic-errors -Wextra -Wconversion -Wsign-conversion -Wno-gnu-zero-variadic-macro-arguments>
    $<$<CXX_COMPILER_ID:MSVC>:
        /WX /W4>
)

######################################################################
if(BUILD_REFERENCE)
    add_library(${PROJECT_NAME}_Reference INTERFACE)
    add_library(Challenge::${PROJECT_NAME}_Reference ALIAS ${PROJECT_NAME}_Reference)

    target_include_directories(${PROJECT_NAME}_Reference INTERFACE src/)

    target_sources(${PROJECT_NAME}_Reference INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/reference.hpp)

    target_compile_definitions(${PROJECT_NAME}_Reference INTERFACE BUILD_REFERENCE)

    target_compile_features(${PROJECT_NAME}_Reference INTERFACE cxx_std_20)

    target_compile_options(${PROJECT_NAME}_Reference INTERFACE
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
            -Wall -Werror -pedantic-errors -Wextra -Wconversion -Wsign-conversion -Wno-gnu-zero-variadic-macro-arguments>
        $<$<CXX_COMPILER_ID:MSVC>:
            /WX /W4>
    )
endif()

######################################################################
if( BUILD_TESTS)
    find_package(gtest REQUIRED)

    add_executable(${PROJECT_NAME}_Tests)
    
    target_sources(${PROJECT_NAME}_Tests PRIVATE 
        src/tests.cpp)

    target_include_directories(${PROJECT_NAME}_Tests PRIVATE
        src/)

    target_link_libraries(${PROJECT_NAME}_Tests PRIVATE
        Challenge::${PROJECT_NAME}
        gtest::gtest
        $<$<TARGET_EXISTS:Challenge::${PROJECT_NAME}_Reference>:Challenge::${PROJECT_NAME}_Reference>)

    target_compile_options(${PROJECT_NAME}_Tests PRIVATE
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:
            -Wno-gnu-zero-variadic-macro-arguments>
    )

    include(GoogleTest)
    enable_testing()
    gtest_discover_tests(${PROJECT_NAME}_Tests)
endif()

######################################################################
if( BUILD_BENCHMARK)
    find_package(benchmark REQUIRED)
    # Requires PThreads on Linux
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)

    add_executable(${PROJECT_NAME}_Benchmark)

    target_sources(${PROJECT_NAME}_Benchmark PRIVATE 
        src/benchmark.cpp)

    target_include_directories(${PROJECT_NAME}_Benchmark PRIVATE
        src/benchmark)

    target_link_libraries(${PROJECT_NAME}_Benchmark PRIVATE
        Challenge::${PROJECT_NAME}
        benchmark::benchmark
        Threads::Threads
        $<$<TARGET_EXISTS:Challenge::${PROJECT_NAME}_Reference>:Challenge::${PROJECT_NAME}_Reference>)

    add_custom_target(NAME ${PROJECT_NAME}_Benchmark
        COMMAND ${PROJECT_NAME}_Benchmark
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
endif()
