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
include api/examples/tutorials/CMakeFiles/client-tutorial2.dir/depend.make

# Include the progress variables for this target.
include api/examples/tutorials/CMakeFiles/client-tutorial2.dir/progress.make

# Include the compile flags for this target's objects.
include api/examples/tutorials/CMakeFiles/client-tutorial2.dir/flags.make

api/examples/tutorials/CMakeFiles/client-tutorial2.dir/client-tutorial2.c.o: api/examples/tutorials/CMakeFiles/client-tutorial2.dir/flags.make
api/examples/tutorials/CMakeFiles/client-tutorial2.dir/client-tutorial2.c.o: ../api/examples/tutorials/client-tutorial2.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object api/examples/tutorials/CMakeFiles/client-tutorial2.dir/client-tutorial2.c.o"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/api/examples/tutorials && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/client-tutorial2.dir/client-tutorial2.c.o   -c /home/tusnio/ci40_project/awacontiki/AwaLWM2M/api/examples/tutorials/client-tutorial2.c

api/examples/tutorials/CMakeFiles/client-tutorial2.dir/client-tutorial2.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/client-tutorial2.dir/client-tutorial2.c.i"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/api/examples/tutorials && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/tusnio/ci40_project/awacontiki/AwaLWM2M/api/examples/tutorials/client-tutorial2.c > CMakeFiles/client-tutorial2.dir/client-tutorial2.c.i

api/examples/tutorials/CMakeFiles/client-tutorial2.dir/client-tutorial2.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/client-tutorial2.dir/client-tutorial2.c.s"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/api/examples/tutorials && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/tusnio/ci40_project/awacontiki/AwaLWM2M/api/examples/tutorials/client-tutorial2.c -o CMakeFiles/client-tutorial2.dir/client-tutorial2.c.s

api/examples/tutorials/CMakeFiles/client-tutorial2.dir/client-tutorial2.c.o.requires:
.PHONY : api/examples/tutorials/CMakeFiles/client-tutorial2.dir/client-tutorial2.c.o.requires

api/examples/tutorials/CMakeFiles/client-tutorial2.dir/client-tutorial2.c.o.provides: api/examples/tutorials/CMakeFiles/client-tutorial2.dir/client-tutorial2.c.o.requires
	$(MAKE) -f api/examples/tutorials/CMakeFiles/client-tutorial2.dir/build.make api/examples/tutorials/CMakeFiles/client-tutorial2.dir/client-tutorial2.c.o.provides.build
.PHONY : api/examples/tutorials/CMakeFiles/client-tutorial2.dir/client-tutorial2.c.o.provides

api/examples/tutorials/CMakeFiles/client-tutorial2.dir/client-tutorial2.c.o.provides.build: api/examples/tutorials/CMakeFiles/client-tutorial2.dir/client-tutorial2.c.o

# Object files for target client-tutorial2
client__tutorial2_OBJECTS = \
"CMakeFiles/client-tutorial2.dir/client-tutorial2.c.o"

# External object files for target client-tutorial2
client__tutorial2_EXTERNAL_OBJECTS =

api/examples/tutorials/client-tutorial2: api/examples/tutorials/CMakeFiles/client-tutorial2.dir/client-tutorial2.c.o
api/examples/tutorials/client-tutorial2: api/examples/tutorials/CMakeFiles/client-tutorial2.dir/build.make
api/examples/tutorials/client-tutorial2: api/src/libawa.a
api/examples/tutorials/client-tutorial2: core/src/client/libawa_static.a
api/examples/tutorials/client-tutorial2: lib/xml/libxml.a
api/examples/tutorials/client-tutorial2: core/src/common/liblwm2mcommon.a
api/examples/tutorials/client-tutorial2: lib/xml/libxml.a
api/examples/tutorials/client-tutorial2: core/src/erbium/liberbiumstatic.a
api/examples/tutorials/client-tutorial2: lib/b64/libb64.a
api/examples/tutorials/client-tutorial2: lib/hmac/libhmac.a
api/examples/tutorials/client-tutorial2: api/examples/tutorials/CMakeFiles/client-tutorial2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable client-tutorial2"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/api/examples/tutorials && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/client-tutorial2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
api/examples/tutorials/CMakeFiles/client-tutorial2.dir/build: api/examples/tutorials/client-tutorial2
.PHONY : api/examples/tutorials/CMakeFiles/client-tutorial2.dir/build

api/examples/tutorials/CMakeFiles/client-tutorial2.dir/requires: api/examples/tutorials/CMakeFiles/client-tutorial2.dir/client-tutorial2.c.o.requires
.PHONY : api/examples/tutorials/CMakeFiles/client-tutorial2.dir/requires

api/examples/tutorials/CMakeFiles/client-tutorial2.dir/clean:
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/api/examples/tutorials && $(CMAKE_COMMAND) -P CMakeFiles/client-tutorial2.dir/cmake_clean.cmake
.PHONY : api/examples/tutorials/CMakeFiles/client-tutorial2.dir/clean

api/examples/tutorials/CMakeFiles/client-tutorial2.dir/depend:
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tusnio/ci40_project/awacontiki/AwaLWM2M /home/tusnio/ci40_project/awacontiki/AwaLWM2M/api/examples/tutorials /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/api/examples/tutorials /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/api/examples/tutorials/CMakeFiles/client-tutorial2.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : api/examples/tutorials/CMakeFiles/client-tutorial2.dir/depend

