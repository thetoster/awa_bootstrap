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
include lib/hmac/CMakeFiles/libhmac_static.dir/depend.make

# Include the progress variables for this target.
include lib/hmac/CMakeFiles/libhmac_static.dir/progress.make

# Include the compile flags for this target's objects.
include lib/hmac/CMakeFiles/libhmac_static.dir/flags.make

lib/hmac/CMakeFiles/libhmac_static.dir/hmac.c.o: lib/hmac/CMakeFiles/libhmac_static.dir/flags.make
lib/hmac/CMakeFiles/libhmac_static.dir/hmac.c.o: ../lib/hmac/hmac.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object lib/hmac/CMakeFiles/libhmac_static.dir/hmac.c.o"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/lib/hmac && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/libhmac_static.dir/hmac.c.o   -c /home/tusnio/ci40_project/awacontiki/AwaLWM2M/lib/hmac/hmac.c

lib/hmac/CMakeFiles/libhmac_static.dir/hmac.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/libhmac_static.dir/hmac.c.i"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/lib/hmac && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/tusnio/ci40_project/awacontiki/AwaLWM2M/lib/hmac/hmac.c > CMakeFiles/libhmac_static.dir/hmac.c.i

lib/hmac/CMakeFiles/libhmac_static.dir/hmac.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/libhmac_static.dir/hmac.c.s"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/lib/hmac && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/tusnio/ci40_project/awacontiki/AwaLWM2M/lib/hmac/hmac.c -o CMakeFiles/libhmac_static.dir/hmac.c.s

lib/hmac/CMakeFiles/libhmac_static.dir/hmac.c.o.requires:
.PHONY : lib/hmac/CMakeFiles/libhmac_static.dir/hmac.c.o.requires

lib/hmac/CMakeFiles/libhmac_static.dir/hmac.c.o.provides: lib/hmac/CMakeFiles/libhmac_static.dir/hmac.c.o.requires
	$(MAKE) -f lib/hmac/CMakeFiles/libhmac_static.dir/build.make lib/hmac/CMakeFiles/libhmac_static.dir/hmac.c.o.provides.build
.PHONY : lib/hmac/CMakeFiles/libhmac_static.dir/hmac.c.o.provides

lib/hmac/CMakeFiles/libhmac_static.dir/hmac.c.o.provides.build: lib/hmac/CMakeFiles/libhmac_static.dir/hmac.c.o

# Object files for target libhmac_static
libhmac_static_OBJECTS = \
"CMakeFiles/libhmac_static.dir/hmac.c.o"

# External object files for target libhmac_static
libhmac_static_EXTERNAL_OBJECTS =

lib/hmac/libhmac.a: lib/hmac/CMakeFiles/libhmac_static.dir/hmac.c.o
lib/hmac/libhmac.a: lib/hmac/CMakeFiles/libhmac_static.dir/build.make
lib/hmac/libhmac.a: lib/hmac/CMakeFiles/libhmac_static.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C static library libhmac.a"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/lib/hmac && $(CMAKE_COMMAND) -P CMakeFiles/libhmac_static.dir/cmake_clean_target.cmake
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/lib/hmac && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/libhmac_static.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
lib/hmac/CMakeFiles/libhmac_static.dir/build: lib/hmac/libhmac.a
.PHONY : lib/hmac/CMakeFiles/libhmac_static.dir/build

lib/hmac/CMakeFiles/libhmac_static.dir/requires: lib/hmac/CMakeFiles/libhmac_static.dir/hmac.c.o.requires
.PHONY : lib/hmac/CMakeFiles/libhmac_static.dir/requires

lib/hmac/CMakeFiles/libhmac_static.dir/clean:
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/lib/hmac && $(CMAKE_COMMAND) -P CMakeFiles/libhmac_static.dir/cmake_clean.cmake
.PHONY : lib/hmac/CMakeFiles/libhmac_static.dir/clean

lib/hmac/CMakeFiles/libhmac_static.dir/depend:
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tusnio/ci40_project/awacontiki/AwaLWM2M /home/tusnio/ci40_project/awacontiki/AwaLWM2M/lib/hmac /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/lib/hmac /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/lib/hmac/CMakeFiles/libhmac_static.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : lib/hmac/CMakeFiles/libhmac_static.dir/depend

