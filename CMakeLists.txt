project(Pongroids)
cmake_minimum_required(VERSION 2.8)

# Set path to additional cmake modules. In this case, we need FindGear2D.cmake
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/" "${CMAKE_SOURCE_DIR}")

# Find Gear2D, we need to link against it.
# If not found, abort.
find_package(Gear2D REQUIRED)

# Add Gear2D include directory
include_directories(${Gear2D_INCLUDE_DIR})

# Add flags to compile using c++11
add_definitions(-std=c++11)

# Change this to suit your root game folder
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/game)

# Macro to help declare a component;
# family is the family of the component, it defines where it will be installed;
# type is the type of the component, it defines the library name;
# sources keeps the source files for this component
macro(gear2d_component family type sources)
	message(STATUS "Component ${family}/${type} from sources ${sources}")
	add_library(${type} MODULE ${sources})
	target_link_libraries(${type} ${Gear2D_LIBRARY})
	install(TARGETS ${type} LIBRARY DESTINATION components/${family})
endmacro()

# Identify if logtrace should be defined
if (${CMAKE_BUILD_TYPE} STREQUAL "Debug" OR ${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
  add_definitions(-DLOGTRACE)
endif(${CMAKE_BUILD_TYPE} STREQUAL "Debug" OR ${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")

# Source code goes in src/
add_subdirectory(src)

