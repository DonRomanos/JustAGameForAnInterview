[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)
<a href="https://travis-ci.com/DonRomanos/JustAGameForAnInterview">![Build Status:](https://travis-ci.com/DonRomanos/JustAGameForAnInterview.svg?label=linux/osx)</a>
<a href="https://ci.appveyor.com/project/DonRomanos/JustAGameForAnInterview" target="_blank">![Build Status:](https://ci.appveyor.com/api/projects/status/github/donromanos/JustAGameForAnInterview?svg=true&label=windows)</a>
[![codecov](https://codecov.io/gh/donromanos/Challenge_Template/branch/master/graph/badge.svg)](https://codecov.io/gh/donromanos/Challenge_Template/)
<a href="https://godbolt.org">![Try it online](https://img.shields.io/badge/try%20it-online-blue.svg)</a>

# A Simple Console Game for a Job Interview

A simple console game that fulfills the following requirements:

* The player controls a hero who fights two monsters: an orc and a dragon.
* The hero has 40 health points, the orc has 7, the dragon has 20.
* Every 1400ms, the orc attacks the hero for 1 damage, which means that the hero loses 1 health point.
* Every 2200ms, the dragon attacks the hero for 3 damage.
* Each time the player types "attack orc" or "attack dragon", the hero attacks the corresponding monster for 2 damage.
* If the orc's or dragon's health points are reduced to zero, it is dead and can neither attack nor be attacked.
* If both monsters die, the player wins the game.
* If the hero's health points are reduced to zero, the player loses the game.
* Display text messages on the console to keep the player informed when something happens, e.g. "Hero hits orc. Orc health is 3". Don't include a GUI.

**Note**: By default this uses C++20 because I want to play around with the newest things.

Requirements:

* CMake
* Conan
* Python (for Conan)

Installing the Libraries using Conan (I recommend using a [conan profile](https://docs.conan.io/en/latest/reference/profiles.html) to specify your settings, e.g. run conan install .. -p gcc-9 ...)

```shell
mkdir build && cd build
conan install .. --build missing
conan build ..
```

Now you should see the library successfully compiling and running the tests.

### Alternative without Conan

Requirements:

* CMake

You can build the library without Conan if you manually install the gtest library.

Just tell Cmake where to find them by setting following CMake Variable

```cmake
gtest_DIR
```

Alternatively you can set the `CMAKE_MODULE_PATH` to the location where your `Findgtest.cmake`.

Then you can use a command line like this

```cmake
cmake .. -G "Unix Makefiles" -D CMAKE_C_COMPILER=gcc-9 -D CMAKE_CXX_COMPILER=g++-9 -DCMAKE_BUILD_TYPE=DEBUG -D CMAKE_MODULE_PATH=$PWD
```

## Developement Experiences

I see three main parts to these requirements:

* Graphical Output
* The game logic itself
* User Input

The biggest problem I see are actually syncing the input and output so that I don't get any interleaved messages and synchronous handling of input.

Possibilities I see:

Running a background thread for input or output, requires multi threading and guarding against race conditions.
Polling the input periodically, however this is not easily done using standard C++ facilities.

### First Try

Tryed with a functional approach storing the gamestate and advancing it every step. Instead of using objects and dynamic dispatch. Makes the tests easier to write I think and can also make the code simpler.

I used c++11 async facilities for handling input and output simultaneously, however with my current implementation using getline in a background thread whatever the player types will be scrambled with the output of the game.

Problems that I would fix with more time:

* Interleaved input / output: switch to querying available characters, store them if there is an output erase everything in the current line and afterwards print whatever the player had written in the next line and continue processing his input. This requires manually assemblying the message.
* Different handling of player and creatures: The only difference is the trigger of the attack, so I would unify the behaviour getting rid of a lot of ifs and only handle the trigger individual for each of them (I would refactor input for this).

These are the main points.

Time spent: 3-4 hours

