cmake_minimum_required(VERSION 3.5)
project(HonaDemo)

# Create an option to switch between a system sdl library and a vendored SDL library
option(HonaDemo "Use vendored libraries" OFF)

if(MYGAME_VENDORED)
    # This assumes you have added SDL as a submodule in vendored/SDL
    add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)
else()
    # 1. Look for a SDL3 package,
    # 2. look for the SDL3-shared component, and
    # 3. fail if the shared component cannot be found.
    find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
endif()

set(CMAKE_CXX_STANDARD 17)

find_package(SDL3 REQUIRED) # ha van CMake config
add_executable(HonaDemo WIN32 main.cpp)
target_include_directories(HonaDemo PRIVATE "C:\\SDL\\include")
target_link_libraries(HonaDemo PRIVATE SDL3::SDL3)
