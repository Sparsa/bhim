# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
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
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/sparsa/Projects/bhim

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/sparsa/Projects/bhim/build

# Include any dependencies generated for this target.
include CMakeFiles/bhim.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/bhim.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/bhim.dir/flags.make

CMakeFiles/bhim.dir/src/tinyxml.cpp.o: CMakeFiles/bhim.dir/flags.make
CMakeFiles/bhim.dir/src/tinyxml.cpp.o: ../src/tinyxml.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sparsa/Projects/bhim/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/bhim.dir/src/tinyxml.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/bhim.dir/src/tinyxml.cpp.o -c /home/sparsa/Projects/bhim/src/tinyxml.cpp

CMakeFiles/bhim.dir/src/tinyxml.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bhim.dir/src/tinyxml.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sparsa/Projects/bhim/src/tinyxml.cpp > CMakeFiles/bhim.dir/src/tinyxml.cpp.i

CMakeFiles/bhim.dir/src/tinyxml.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bhim.dir/src/tinyxml.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sparsa/Projects/bhim/src/tinyxml.cpp -o CMakeFiles/bhim.dir/src/tinyxml.cpp.s

CMakeFiles/bhim.dir/src/tinystr.cpp.o: CMakeFiles/bhim.dir/flags.make
CMakeFiles/bhim.dir/src/tinystr.cpp.o: ../src/tinystr.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sparsa/Projects/bhim/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/bhim.dir/src/tinystr.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/bhim.dir/src/tinystr.cpp.o -c /home/sparsa/Projects/bhim/src/tinystr.cpp

CMakeFiles/bhim.dir/src/tinystr.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bhim.dir/src/tinystr.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sparsa/Projects/bhim/src/tinystr.cpp > CMakeFiles/bhim.dir/src/tinystr.cpp.i

CMakeFiles/bhim.dir/src/tinystr.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bhim.dir/src/tinystr.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sparsa/Projects/bhim/src/tinystr.cpp -o CMakeFiles/bhim.dir/src/tinystr.cpp.s

CMakeFiles/bhim.dir/src/tinyxmlerror.cpp.o: CMakeFiles/bhim.dir/flags.make
CMakeFiles/bhim.dir/src/tinyxmlerror.cpp.o: ../src/tinyxmlerror.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sparsa/Projects/bhim/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/bhim.dir/src/tinyxmlerror.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/bhim.dir/src/tinyxmlerror.cpp.o -c /home/sparsa/Projects/bhim/src/tinyxmlerror.cpp

CMakeFiles/bhim.dir/src/tinyxmlerror.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bhim.dir/src/tinyxmlerror.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sparsa/Projects/bhim/src/tinyxmlerror.cpp > CMakeFiles/bhim.dir/src/tinyxmlerror.cpp.i

CMakeFiles/bhim.dir/src/tinyxmlerror.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bhim.dir/src/tinyxmlerror.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sparsa/Projects/bhim/src/tinyxmlerror.cpp -o CMakeFiles/bhim.dir/src/tinyxmlerror.cpp.s

CMakeFiles/bhim.dir/src/tinyxmlparser.cpp.o: CMakeFiles/bhim.dir/flags.make
CMakeFiles/bhim.dir/src/tinyxmlparser.cpp.o: ../src/tinyxmlparser.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sparsa/Projects/bhim/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/bhim.dir/src/tinyxmlparser.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/bhim.dir/src/tinyxmlparser.cpp.o -c /home/sparsa/Projects/bhim/src/tinyxmlparser.cpp

CMakeFiles/bhim.dir/src/tinyxmlparser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bhim.dir/src/tinyxmlparser.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sparsa/Projects/bhim/src/tinyxmlparser.cpp > CMakeFiles/bhim.dir/src/tinyxmlparser.cpp.i

CMakeFiles/bhim.dir/src/tinyxmlparser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bhim.dir/src/tinyxmlparser.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sparsa/Projects/bhim/src/tinyxmlparser.cpp -o CMakeFiles/bhim.dir/src/tinyxmlparser.cpp.s

CMakeFiles/bhim.dir/src/automata.cpp.o: CMakeFiles/bhim.dir/flags.make
CMakeFiles/bhim.dir/src/automata.cpp.o: ../src/automata.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sparsa/Projects/bhim/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/bhim.dir/src/automata.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/bhim.dir/src/automata.cpp.o -c /home/sparsa/Projects/bhim/src/automata.cpp

CMakeFiles/bhim.dir/src/automata.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bhim.dir/src/automata.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sparsa/Projects/bhim/src/automata.cpp > CMakeFiles/bhim.dir/src/automata.cpp.i

CMakeFiles/bhim.dir/src/automata.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bhim.dir/src/automata.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sparsa/Projects/bhim/src/automata.cpp -o CMakeFiles/bhim.dir/src/automata.cpp.s

CMakeFiles/bhim.dir/src/concatenationMultiStack.cpp.o: CMakeFiles/bhim.dir/flags.make
CMakeFiles/bhim.dir/src/concatenationMultiStack.cpp.o: ../src/concatenationMultiStack.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sparsa/Projects/bhim/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/bhim.dir/src/concatenationMultiStack.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/bhim.dir/src/concatenationMultiStack.cpp.o -c /home/sparsa/Projects/bhim/src/concatenationMultiStack.cpp

CMakeFiles/bhim.dir/src/concatenationMultiStack.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bhim.dir/src/concatenationMultiStack.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sparsa/Projects/bhim/src/concatenationMultiStack.cpp > CMakeFiles/bhim.dir/src/concatenationMultiStack.cpp.i

CMakeFiles/bhim.dir/src/concatenationMultiStack.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bhim.dir/src/concatenationMultiStack.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sparsa/Projects/bhim/src/concatenationMultiStack.cpp -o CMakeFiles/bhim.dir/src/concatenationMultiStack.cpp.s

CMakeFiles/bhim.dir/src/main.cpp.o: CMakeFiles/bhim.dir/flags.make
CMakeFiles/bhim.dir/src/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sparsa/Projects/bhim/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/bhim.dir/src/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/bhim.dir/src/main.cpp.o -c /home/sparsa/Projects/bhim/src/main.cpp

CMakeFiles/bhim.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bhim.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sparsa/Projects/bhim/src/main.cpp > CMakeFiles/bhim.dir/src/main.cpp.i

CMakeFiles/bhim.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bhim.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sparsa/Projects/bhim/src/main.cpp -o CMakeFiles/bhim.dir/src/main.cpp.s

# Object files for target bhim
bhim_OBJECTS = \
"CMakeFiles/bhim.dir/src/tinyxml.cpp.o" \
"CMakeFiles/bhim.dir/src/tinystr.cpp.o" \
"CMakeFiles/bhim.dir/src/tinyxmlerror.cpp.o" \
"CMakeFiles/bhim.dir/src/tinyxmlparser.cpp.o" \
"CMakeFiles/bhim.dir/src/automata.cpp.o" \
"CMakeFiles/bhim.dir/src/concatenationMultiStack.cpp.o" \
"CMakeFiles/bhim.dir/src/main.cpp.o"

# External object files for target bhim
bhim_EXTERNAL_OBJECTS =

bhim: CMakeFiles/bhim.dir/src/tinyxml.cpp.o
bhim: CMakeFiles/bhim.dir/src/tinystr.cpp.o
bhim: CMakeFiles/bhim.dir/src/tinyxmlerror.cpp.o
bhim: CMakeFiles/bhim.dir/src/tinyxmlparser.cpp.o
bhim: CMakeFiles/bhim.dir/src/automata.cpp.o
bhim: CMakeFiles/bhim.dir/src/concatenationMultiStack.cpp.o
bhim: CMakeFiles/bhim.dir/src/main.cpp.o
bhim: CMakeFiles/bhim.dir/build.make
bhim: CMakeFiles/bhim.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/sparsa/Projects/bhim/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking CXX executable bhim"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/bhim.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/bhim.dir/build: bhim

.PHONY : CMakeFiles/bhim.dir/build

CMakeFiles/bhim.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/bhim.dir/cmake_clean.cmake
.PHONY : CMakeFiles/bhim.dir/clean

CMakeFiles/bhim.dir/depend:
	cd /home/sparsa/Projects/bhim/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/sparsa/Projects/bhim /home/sparsa/Projects/bhim /home/sparsa/Projects/bhim/build /home/sparsa/Projects/bhim/build /home/sparsa/Projects/bhim/build/CMakeFiles/bhim.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/bhim.dir/depend

