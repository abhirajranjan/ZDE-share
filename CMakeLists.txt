cmake_minimum_required (VERSION 3.8)

project ("aloraShare")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Add source to this project's executable.
add_executable (aloraShare
                "src/network/base.cpp" 
                "src/network/base.hpp"
                "src/network/main.cpp" 
                "src/network/main.hpp" 
                "src/network/tcp.cpp" 
                "src/network/tcp.hpp" 
                "src/network/udp.hpp" 
                "src/network/udp.cpp")
# TODO: Add tests and install targets if needed.