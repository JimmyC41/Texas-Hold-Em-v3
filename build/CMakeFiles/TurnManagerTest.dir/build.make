# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.30

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
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.30.3/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.30.3/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/jimmy/Texas-Hold-Em-v3

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/jimmy/Texas-Hold-Em-v3/build

# Include any dependencies generated for this target.
include CMakeFiles/TurnManagerTest.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/TurnManagerTest.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/TurnManagerTest.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/TurnManagerTest.dir/flags.make

CMakeFiles/TurnManagerTest.dir/test/TurnManagerTest.cpp.o: CMakeFiles/TurnManagerTest.dir/flags.make
CMakeFiles/TurnManagerTest.dir/test/TurnManagerTest.cpp.o: /Users/jimmy/Texas-Hold-Em-v3/test/TurnManagerTest.cpp
CMakeFiles/TurnManagerTest.dir/test/TurnManagerTest.cpp.o: CMakeFiles/TurnManagerTest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/jimmy/Texas-Hold-Em-v3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/TurnManagerTest.dir/test/TurnManagerTest.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TurnManagerTest.dir/test/TurnManagerTest.cpp.o -MF CMakeFiles/TurnManagerTest.dir/test/TurnManagerTest.cpp.o.d -o CMakeFiles/TurnManagerTest.dir/test/TurnManagerTest.cpp.o -c /Users/jimmy/Texas-Hold-Em-v3/test/TurnManagerTest.cpp

CMakeFiles/TurnManagerTest.dir/test/TurnManagerTest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/TurnManagerTest.dir/test/TurnManagerTest.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jimmy/Texas-Hold-Em-v3/test/TurnManagerTest.cpp > CMakeFiles/TurnManagerTest.dir/test/TurnManagerTest.cpp.i

CMakeFiles/TurnManagerTest.dir/test/TurnManagerTest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/TurnManagerTest.dir/test/TurnManagerTest.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jimmy/Texas-Hold-Em-v3/test/TurnManagerTest.cpp -o CMakeFiles/TurnManagerTest.dir/test/TurnManagerTest.cpp.s

# Object files for target TurnManagerTest
TurnManagerTest_OBJECTS = \
"CMakeFiles/TurnManagerTest.dir/test/TurnManagerTest.cpp.o"

# External object files for target TurnManagerTest
TurnManagerTest_EXTERNAL_OBJECTS =

TurnManagerTest: CMakeFiles/TurnManagerTest.dir/test/TurnManagerTest.cpp.o
TurnManagerTest: CMakeFiles/TurnManagerTest.dir/build.make
TurnManagerTest: libPokerLib.a
TurnManagerTest: /opt/homebrew/lib/libgtest_main.a
TurnManagerTest: /opt/homebrew/lib/libgtest.a
TurnManagerTest: CMakeFiles/TurnManagerTest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/jimmy/Texas-Hold-Em-v3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable TurnManagerTest"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/TurnManagerTest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/TurnManagerTest.dir/build: TurnManagerTest
.PHONY : CMakeFiles/TurnManagerTest.dir/build

CMakeFiles/TurnManagerTest.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/TurnManagerTest.dir/cmake_clean.cmake
.PHONY : CMakeFiles/TurnManagerTest.dir/clean

CMakeFiles/TurnManagerTest.dir/depend:
	cd /Users/jimmy/Texas-Hold-Em-v3/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/jimmy/Texas-Hold-Em-v3 /Users/jimmy/Texas-Hold-Em-v3 /Users/jimmy/Texas-Hold-Em-v3/build /Users/jimmy/Texas-Hold-Em-v3/build /Users/jimmy/Texas-Hold-Em-v3/build/CMakeFiles/TurnManagerTest.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/TurnManagerTest.dir/depend

