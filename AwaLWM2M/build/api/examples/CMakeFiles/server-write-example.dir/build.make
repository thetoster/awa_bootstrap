# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_SOURCE_DIR = /home/tusnio/ci40_project/awacontiki/AwaLWM2M

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build

# Include any dependencies generated for this target.
include api/examples/CMakeFiles/server-write-example.dir/depend.make

# Include the progress variables for this target.
include api/examples/CMakeFiles/server-write-example.dir/progress.make

# Include the compile flags for this target's objects.
include api/examples/CMakeFiles/server-write-example.dir/flags.make

api/examples/CMakeFiles/server-write-example.dir/server-write-example.c.o: api/examples/CMakeFiles/server-write-example.dir/flags.make
api/examples/CMakeFiles/server-write-example.dir/server-write-example.c.o: ../api/examples/server-write-example.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object api/examples/CMakeFiles/server-write-example.dir/server-write-example.c.o"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/api/examples && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/server-write-example.dir/server-write-example.c.o   -c /home/tusnio/ci40_project/awacontiki/AwaLWM2M/api/examples/server-write-example.c

api/examples/CMakeFiles/server-write-example.dir/server-write-example.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/server-write-example.dir/server-write-example.c.i"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/api/examples && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/tusnio/ci40_project/awacontiki/AwaLWM2M/api/examples/server-write-example.c > CMakeFiles/server-write-example.dir/server-write-example.c.i

api/examples/CMakeFiles/server-write-example.dir/server-write-example.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/server-write-example.dir/server-write-example.c.s"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/api/examples && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/tusnio/ci40_project/awacontiki/AwaLWM2M/api/examples/server-write-example.c -o CMakeFiles/server-write-example.dir/server-write-example.c.s

api/examples/CMakeFiles/server-write-example.dir/server-write-example.c.o.requires:
.PHONY : api/examples/CMakeFiles/server-write-example.dir/server-write-example.c.o.requires

api/examples/CMakeFiles/server-write-example.dir/server-write-example.c.o.provides: api/examples/CMakeFiles/server-write-example.dir/server-write-example.c.o.requires
	$(MAKE) -f api/examples/CMakeFiles/server-write-example.dir/build.make api/examples/CMakeFiles/server-write-example.dir/server-write-example.c.o.provides.build
.PHONY : api/examples/CMakeFiles/server-write-example.dir/server-write-example.c.o.provides

api/examples/CMakeFiles/server-write-example.dir/server-write-example.c.o.provides.build: api/examples/CMakeFiles/server-write-example.dir/server-write-example.c.o

# Object files for target server-write-example
server__write__example_OBJECTS = \
"CMakeFiles/server-write-example.dir/server-write-example.c.o"

# External object files for target server-write-example
server__write__example_EXTERNAL_OBJECTS =

api/examples/server-write-example: api/examples/CMakeFiles/server-write-example.dir/server-write-example.c.o
api/examples/server-write-example: api/examples/CMakeFiles/server-write-example.dir/build.make
api/examples/server-write-example: api/src/libawa.a
api/examples/server-write-example: lib/xml/libxml.a
api/examples/server-write-example: lib/b64/libb64.a
api/examples/server-write-example: lib/hmac/libhmac.a
api/examples/server-write-example: api/examples/CMakeFiles/server-write-example.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable server-write-example"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/api/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/server-write-example.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
api/examples/CMakeFiles/server-write-example.dir/build: api/examples/server-write-example
.PHONY : api/examples/CMakeFiles/server-write-example.dir/build

api/examples/CMakeFiles/server-write-example.dir/requires: api/examples/CMakeFiles/server-write-example.dir/server-write-example.c.o.requires
.PHONY : api/examples/CMakeFiles/server-write-example.dir/requires

api/examples/CMakeFiles/server-write-example.dir/clean:
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/api/examples && $(CMAKE_COMMAND) -P CMakeFiles/server-write-example.dir/cmake_clean.cmake
.PHONY : api/examples/CMakeFiles/server-write-example.dir/clean

api/examples/CMakeFiles/server-write-example.dir/depend:
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tusnio/ci40_project/awacontiki/AwaLWM2M /home/tusnio/ci40_project/awacontiki/AwaLWM2M/api/examples /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/api/examples /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/api/examples/CMakeFiles/server-write-example.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : api/examples/CMakeFiles/server-write-example.dir/depend

