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
include CMakeFiles/DealTest.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/DealTest.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/DealTest.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/DealTest.dir/flags.make

CMakeFiles/DealTest.dir/test/DealTest.cpp.o: CMakeFiles/DealTest.dir/flags.make
CMakeFiles/DealTest.dir/test/DealTest.cpp.o: /Users/jimmy/Texas-Hold-Em-v3/test/DealTest.cpp
CMakeFiles/DealTest.dir/test/DealTest.cpp.o: CMakeFiles/DealTest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/jimmy/Texas-Hold-Em-v3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/DealTest.dir/test/DealTest.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/DealTest.dir/test/DealTest.cpp.o -MF CMakeFiles/DealTest.dir/test/DealTest.cpp.o.d -o CMakeFiles/DealTest.dir/test/DealTest.cpp.o -c /Users/jimmy/Texas-Hold-Em-v3/test/DealTest.cpp

CMakeFiles/DealTest.dir/test/DealTest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/DealTest.dir/test/DealTest.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jimmy/Texas-Hold-Em-v3/test/DealTest.cpp > CMakeFiles/DealTest.dir/test/DealTest.cpp.i

CMakeFiles/DealTest.dir/test/DealTest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/DealTest.dir/test/DealTest.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jimmy/Texas-Hold-Em-v3/test/DealTest.cpp -o CMakeFiles/DealTest.dir/test/DealTest.cpp.s

# Object files for target DealTest
DealTest_OBJECTS = \
"CMakeFiles/DealTest.dir/test/DealTest.cpp.o"

# External object files for target DealTest
DealTest_EXTERNAL_OBJECTS =

DealTest: CMakeFiles/DealTest.dir/test/DealTest.cpp.o
DealTest: CMakeFiles/DealTest.dir/build.make
DealTest: libPokerLib.a
DealTest: /opt/homebrew/lib/libgtest_main.a
DealTest: /opt/homebrew/lib/libgtest.a
DealTest: CMakeFiles/DealTest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/jimmy/Texas-Hold-Em-v3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable DealTest"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/DealTest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/DealTest.dir/build: DealTest
.PHONY : CMakeFiles/DealTest.dir/build

CMakeFiles/DealTest.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/DealTest.dir/cmake_clean.cmake
.PHONY : CMakeFiles/DealTest.dir/clean

CMakeFiles/DealTest.dir/depend:
	cd /Users/jimmy/Texas-Hold-Em-v3/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/jimmy/Texas-Hold-Em-v3 /Users/jimmy/Texas-Hold-Em-v3 /Users/jimmy/Texas-Hold-Em-v3/build /Users/jimmy/Texas-Hold-Em-v3/build /Users/jimmy/Texas-Hold-Em-v3/build/CMakeFiles/DealTest.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/DealTest.dir/depend

