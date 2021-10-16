cmake_minimum_required(VERSION 3.0.0)
project(my_sfml_template VERSION 0.1.0)

include(FetchContent)

set(BUILD_SHARED_LIBS OFF)
FetchContent_Declare(
	SFML
	GIT_REPOSITORY https://github.com/SFML/SFML.git
	GIT_TAG 2.5.1
)
FetchContent_MakeAvailable(SFML)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -ggdb -Wall -Wextra -Wshadow -pedantic")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

set(SOURCES
    src/game.cpp
    src/random_generator.cpp
    src/walker.cpp
    src/main.cpp
)

add_executable(game ${SOURCES})

target_include_directories(game PUBLIC include)

target_link_libraries(
	game
	sfml-graphics
    sfml-system
    sfml-window
)

add_custom_command(TARGET game POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/assets/ $<TARGET_FILE_DIR:game>/assets/)


