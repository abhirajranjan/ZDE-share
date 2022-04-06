# CMakeList.txt : CMake project for CMakeProject2, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("aloraShare")

# Add source to this project's executable.
add_executable (aloraShare "network/base.cpp" "network/base.hpp" "network/main.cpp" "network/main.hpp" "network/tcp.cpp" "network/tcp.hpp" "network/udp.hpp" "network/udp.cpp")
# TODO: Add tests and install targets if needed.
