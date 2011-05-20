project(pongroids-components)

add_library(pong SHARED pong.cc)
add_library(asteroid SHARED asteroid.cc)

install(TARGETS asteroid
		LIBRARY DESTINATION ../../../game/com/asteroid)

install(TARGETS pong
		LIBRARY DESTINATION ../../../game/com/pong)


target_link_libraries(pong gear2d)

