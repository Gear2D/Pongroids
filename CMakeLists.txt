project(Pongroids)
cmake_minimum_required(VERSION 2.8)

# Find Gear2D, we need to link against it.
# If not found, abort.
find_package(Gear2D REQUIRED)

# Add Gear2D include directory
include_directories(${Gear2D_INCLUDE_DIR})

# Add flags to compile using c++11
add_definitions(-std=c++11)

# Change this to suit your root game folder
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/game)

# Identify if logtrace should be defined
if (${CMAKE_BUILD_TYPE} STREQUAL "Debug" OR ${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
  add_definitions(-DLOGTRACE)
endif(${CMAKE_BUILD_TYPE} STREQUAL "Debug" OR ${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")

if (MINGW)
  message(STATUS "Compiling with MINGW!")
  set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static -lpthread -dynamic")
  set(CMAKE_SHARED_LINKER_FLAGS "-static-libstdc++ -static-libgcc -static -lpthread -dynamic")
  set(CMAKE_MODULE_LINKER_FLAGS "-static-libstdc++ -static-libgcc -static -lpthread -dynamic")
endif(MINGW)

# Source code goes in src/
add_subdirectory(src)

