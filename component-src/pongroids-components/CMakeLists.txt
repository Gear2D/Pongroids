project(pongroids-components)

set(CMAKE_INSTALL_PREFIX ./)

add_library(pong SHARED pong.cc)
add_library(asteroid SHARED asteroid.cc)
add_library(bar SHARED bar.cc)
add_library(match SHARED match.cc)
add_library(entrance SHARED entrance.cc)


install(TARGETS asteroid
		LIBRARY DESTINATION ../../../game/com/asteroid)

install(TARGETS pong
		LIBRARY DESTINATION ../../../game/com/pong)

install(TARGETS bar
		LIBRARY DESTINATION ../../../game/com/bar)

install(TARGETS match
		LIBRARY DESTINATION ../../../game/com/match)

install(TARGETS entrance
		LIBRARY DESTINATION ../../../game/com/entrance)

target_link_libraries(pong gear2d)
target_link_libraries(bar gear2d)
target_link_libraries(asteroid gear2d)

