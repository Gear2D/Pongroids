project(pongroids-components)

set(CMAKE_INSTALL_PREFIX ./)

find_path(GEAR2D_INCLUDE_DIR gear2d/gear2d.h)
find_path(GEAR2D_LINK_DIR GLOB libgear2d.*)
include_directories(${GEAR2D_INCLUDE_DIR})
link_directories(${GEAR2D_LINK_DIR})

add_library(pong SHARED pong.cc)
add_library(asteroid SHARED asteroid.cc)
add_library(bar SHARED bar.cc)
add_library(aibar SHARED aibar.cc)
add_library(match SHARED match.cc)
add_library(entrance SHARED entrance.cc)


install(TARGETS asteroid
		 DESTINATION ../../../game/com/asteroid)

install(TARGETS pong
		 DESTINATION ../../../game/com/pong)

install(TARGETS bar
		 DESTINATION ../../../game/com/bar)

install(TARGETS aibar
		 DESTINATION ../../../game/com/bar)

install(TARGETS match
		 DESTINATION ../../../game/com/match)

install(TARGETS entrance
		 DESTINATION ../../../game/com/entrance)

target_link_libraries(pong gear2d)
target_link_libraries(bar gear2d)
target_link_libraries(aibar gear2d)
target_link_libraries(asteroid gear2d)
target_link_libraries(match gear2d)
target_link_libraries(entrance gear2d)
