cmake_minimum_required (VERSION 3.8)

if(WIN32)
    message(STATUS "building for WINDOWS System")
elseif(UNIX)
    message(STATUS "building for POSIX System")
else()
    message(STATUS "Cannot build required modules does not match")
endif()

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/src/aloraShare/include/platform.hpp.in ${CMAKE_CURRENT_SOURCE_DIR}/src/aloraShare/include/platform.hpp)

project ("aloraShare")

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

include_directories("src/aloraShare/include")

# Add source to this project's executable.
add_executable (aloraShare
"src/aloraShare/base.cpp" 
"src/aloraShare/aloraShare.cpp"  
"src/aloraShare/tcp.cpp" 
"src/aloraShare/udp.cpp"
"src/daemon.cpp"
)

# TODO: Add tests and install targets if needed.
target_link_libraries(aloraShare Threads::Threads)