cmake_minimum_required(VERSION 3.23)

project(simple_echo_server)

set(CMAKE_CXX_STANDARD 20)

#add_subdirectory(lib/uWebSocket/uSockets)

include_directories(
        lib/uWebSocket/src
        lib/uWebSocket/uSockets/src
)

link_directories(
#        /home/krishan/Trainings/C++/uWebSockets/uSockets
        lib/uWebSocket/uSockets/static
)

add_executable(simple_echo_server main.cpp )

target_link_libraries(
        simple_echo_server
        uSockets
        z
)