# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /storage1/projects/alora-share/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /storage1/projects/alora-share

# Include any dependencies generated for this target.
include CMakeFiles/aloraShare.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/aloraShare.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/aloraShare.dir/flags.make

CMakeFiles/aloraShare.dir/network/base.cpp.o: CMakeFiles/aloraShare.dir/flags.make
CMakeFiles/aloraShare.dir/network/base.cpp.o: src/network/base.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/storage1/projects/alora-share/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/aloraShare.dir/network/base.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/aloraShare.dir/network/base.cpp.o -c /storage1/projects/alora-share/src/network/base.cpp

CMakeFiles/aloraShare.dir/network/base.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/aloraShare.dir/network/base.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /storage1/projects/alora-share/src/network/base.cpp > CMakeFiles/aloraShare.dir/network/base.cpp.i

CMakeFiles/aloraShare.dir/network/base.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/aloraShare.dir/network/base.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /storage1/projects/alora-share/src/network/base.cpp -o CMakeFiles/aloraShare.dir/network/base.cpp.s

CMakeFiles/aloraShare.dir/network/main.cpp.o: CMakeFiles/aloraShare.dir/flags.make
CMakeFiles/aloraShare.dir/network/main.cpp.o: src/network/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/storage1/projects/alora-share/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/aloraShare.dir/network/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/aloraShare.dir/network/main.cpp.o -c /storage1/projects/alora-share/src/network/main.cpp

CMakeFiles/aloraShare.dir/network/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/aloraShare.dir/network/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /storage1/projects/alora-share/src/network/main.cpp > CMakeFiles/aloraShare.dir/network/main.cpp.i

CMakeFiles/aloraShare.dir/network/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/aloraShare.dir/network/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /storage1/projects/alora-share/src/network/main.cpp -o CMakeFiles/aloraShare.dir/network/main.cpp.s

CMakeFiles/aloraShare.dir/network/tcp.cpp.o: CMakeFiles/aloraShare.dir/flags.make
CMakeFiles/aloraShare.dir/network/tcp.cpp.o: src/network/tcp.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/storage1/projects/alora-share/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/aloraShare.dir/network/tcp.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/aloraShare.dir/network/tcp.cpp.o -c /storage1/projects/alora-share/src/network/tcp.cpp

CMakeFiles/aloraShare.dir/network/tcp.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/aloraShare.dir/network/tcp.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /storage1/projects/alora-share/src/network/tcp.cpp > CMakeFiles/aloraShare.dir/network/tcp.cpp.i

CMakeFiles/aloraShare.dir/network/tcp.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/aloraShare.dir/network/tcp.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /storage1/projects/alora-share/src/network/tcp.cpp -o CMakeFiles/aloraShare.dir/network/tcp.cpp.s

CMakeFiles/aloraShare.dir/network/udp.cpp.o: CMakeFiles/aloraShare.dir/flags.make
CMakeFiles/aloraShare.dir/network/udp.cpp.o: src/network/udp.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/storage1/projects/alora-share/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/aloraShare.dir/network/udp.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/aloraShare.dir/network/udp.cpp.o -c /storage1/projects/alora-share/src/network/udp.cpp

CMakeFiles/aloraShare.dir/network/udp.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/aloraShare.dir/network/udp.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /storage1/projects/alora-share/src/network/udp.cpp > CMakeFiles/aloraShare.dir/network/udp.cpp.i

CMakeFiles/aloraShare.dir/network/udp.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/aloraShare.dir/network/udp.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /storage1/projects/alora-share/src/network/udp.cpp -o CMakeFiles/aloraShare.dir/network/udp.cpp.s

# Object files for target aloraShare
aloraShare_OBJECTS = \
"CMakeFiles/aloraShare.dir/network/base.cpp.o" \
"CMakeFiles/aloraShare.dir/network/main.cpp.o" \
"CMakeFiles/aloraShare.dir/network/tcp.cpp.o" \
"CMakeFiles/aloraShare.dir/network/udp.cpp.o"

# External object files for target aloraShare
aloraShare_EXTERNAL_OBJECTS =

aloraShare: CMakeFiles/aloraShare.dir/network/base.cpp.o
aloraShare: CMakeFiles/aloraShare.dir/network/main.cpp.o
aloraShare: CMakeFiles/aloraShare.dir/network/tcp.cpp.o
aloraShare: CMakeFiles/aloraShare.dir/network/udp.cpp.o
aloraShare: CMakeFiles/aloraShare.dir/build.make
aloraShare: CMakeFiles/aloraShare.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/storage1/projects/alora-share/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable aloraShare"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/aloraShare.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/aloraShare.dir/build: aloraShare

.PHONY : CMakeFiles/aloraShare.dir/build

CMakeFiles/aloraShare.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/aloraShare.dir/cmake_clean.cmake
.PHONY : CMakeFiles/aloraShare.dir/clean

CMakeFiles/aloraShare.dir/depend:
	cd /storage1/projects/alora-share && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /storage1/projects/alora-share/src /storage1/projects/alora-share/src /storage1/projects/alora-share /storage1/projects/alora-share /storage1/projects/alora-share/CMakeFiles/aloraShare.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/aloraShare.dir/depend

