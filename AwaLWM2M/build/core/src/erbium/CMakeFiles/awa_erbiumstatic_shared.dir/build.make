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
include core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/depend.make

# Include the progress variables for this target.
include core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/progress.make

# Include the compile flags for this target's objects.
include core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/flags.make

core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap.c.o: core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/flags.make
core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap.c.o: ../core/src/erbium/er-coap.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap.c.o"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/core/src/erbium && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/awa_erbiumstatic_shared.dir/er-coap.c.o   -c /home/tusnio/ci40_project/awacontiki/AwaLWM2M/core/src/erbium/er-coap.c

core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/awa_erbiumstatic_shared.dir/er-coap.c.i"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/core/src/erbium && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/tusnio/ci40_project/awacontiki/AwaLWM2M/core/src/erbium/er-coap.c > CMakeFiles/awa_erbiumstatic_shared.dir/er-coap.c.i

core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/awa_erbiumstatic_shared.dir/er-coap.c.s"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/core/src/erbium && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/tusnio/ci40_project/awacontiki/AwaLWM2M/core/src/erbium/er-coap.c -o CMakeFiles/awa_erbiumstatic_shared.dir/er-coap.c.s

core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap.c.o.requires:
.PHONY : core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap.c.o.requires

core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap.c.o.provides: core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap.c.o.requires
	$(MAKE) -f core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/build.make core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap.c.o.provides.build
.PHONY : core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap.c.o.provides

core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap.c.o.provides.build: core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap.c.o

core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-engine.c.o: core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/flags.make
core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-engine.c.o: ../core/src/erbium/er-coap-engine.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-engine.c.o"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/core/src/erbium && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-engine.c.o   -c /home/tusnio/ci40_project/awacontiki/AwaLWM2M/core/src/erbium/er-coap-engine.c

core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-engine.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-engine.c.i"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/core/src/erbium && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/tusnio/ci40_project/awacontiki/AwaLWM2M/core/src/erbium/er-coap-engine.c > CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-engine.c.i

core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-engine.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-engine.c.s"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/core/src/erbium && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/tusnio/ci40_project/awacontiki/AwaLWM2M/core/src/erbium/er-coap-engine.c -o CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-engine.c.s

core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-engine.c.o.requires:
.PHONY : core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-engine.c.o.requires

core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-engine.c.o.provides: core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-engine.c.o.requires
	$(MAKE) -f core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/build.make core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-engine.c.o.provides.build
.PHONY : core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-engine.c.o.provides

core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-engine.c.o.provides.build: core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-engine.c.o

core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-transactions.c.o: core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/flags.make
core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-transactions.c.o: ../core/src/erbium/er-coap-transactions.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-transactions.c.o"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/core/src/erbium && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-transactions.c.o   -c /home/tusnio/ci40_project/awacontiki/AwaLWM2M/core/src/erbium/er-coap-transactions.c

core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-transactions.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-transactions.c.i"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/core/src/erbium && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/tusnio/ci40_project/awacontiki/AwaLWM2M/core/src/erbium/er-coap-transactions.c > CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-transactions.c.i

core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-transactions.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-transactions.c.s"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/core/src/erbium && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/tusnio/ci40_project/awacontiki/AwaLWM2M/core/src/erbium/er-coap-transactions.c -o CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-transactions.c.s

core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-transactions.c.o.requires:
.PHONY : core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-transactions.c.o.requires

core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-transactions.c.o.provides: core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-transactions.c.o.requires
	$(MAKE) -f core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/build.make core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-transactions.c.o.provides.build
.PHONY : core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-transactions.c.o.provides

core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-transactions.c.o.provides.build: core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-transactions.c.o

core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-separate.c.o: core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/flags.make
core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-separate.c.o: ../core/src/erbium/er-coap-separate.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-separate.c.o"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/core/src/erbium && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-separate.c.o   -c /home/tusnio/ci40_project/awacontiki/AwaLWM2M/core/src/erbium/er-coap-separate.c

core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-separate.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-separate.c.i"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/core/src/erbium && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/tusnio/ci40_project/awacontiki/AwaLWM2M/core/src/erbium/er-coap-separate.c > CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-separate.c.i

core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-separate.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-separate.c.s"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/core/src/erbium && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/tusnio/ci40_project/awacontiki/AwaLWM2M/core/src/erbium/er-coap-separate.c -o CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-separate.c.s

core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-separate.c.o.requires:
.PHONY : core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-separate.c.o.requires

core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-separate.c.o.provides: core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-separate.c.o.requires
	$(MAKE) -f core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/build.make core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-separate.c.o.provides.build
.PHONY : core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-separate.c.o.provides

core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-separate.c.o.provides.build: core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-separate.c.o

core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-block1.c.o: core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/flags.make
core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-block1.c.o: ../core/src/erbium/er-coap-block1.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-block1.c.o"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/core/src/erbium && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-block1.c.o   -c /home/tusnio/ci40_project/awacontiki/AwaLWM2M/core/src/erbium/er-coap-block1.c

core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-block1.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-block1.c.i"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/core/src/erbium && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/tusnio/ci40_project/awacontiki/AwaLWM2M/core/src/erbium/er-coap-block1.c > CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-block1.c.i

core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-block1.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-block1.c.s"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/core/src/erbium && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/tusnio/ci40_project/awacontiki/AwaLWM2M/core/src/erbium/er-coap-block1.c -o CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-block1.c.s

core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-block1.c.o.requires:
.PHONY : core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-block1.c.o.requires

core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-block1.c.o.provides: core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-block1.c.o.requires
	$(MAKE) -f core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/build.make core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-block1.c.o.provides.build
.PHONY : core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-block1.c.o.provides

core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-block1.c.o.provides.build: core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-block1.c.o

# Object files for target awa_erbiumstatic_shared
awa_erbiumstatic_shared_OBJECTS = \
"CMakeFiles/awa_erbiumstatic_shared.dir/er-coap.c.o" \
"CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-engine.c.o" \
"CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-transactions.c.o" \
"CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-separate.c.o" \
"CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-block1.c.o"

# External object files for target awa_erbiumstatic_shared
awa_erbiumstatic_shared_EXTERNAL_OBJECTS =

core/src/erbium/liberbiumstatic.so: core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap.c.o
core/src/erbium/liberbiumstatic.so: core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-engine.c.o
core/src/erbium/liberbiumstatic.so: core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-transactions.c.o
core/src/erbium/liberbiumstatic.so: core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-separate.c.o
core/src/erbium/liberbiumstatic.so: core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-block1.c.o
core/src/erbium/liberbiumstatic.so: core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/build.make
core/src/erbium/liberbiumstatic.so: lib/b64/libb64.a
core/src/erbium/liberbiumstatic.so: lib/hmac/libhmac.a
core/src/erbium/liberbiumstatic.so: core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C shared library liberbiumstatic.so"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/core/src/erbium && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/awa_erbiumstatic_shared.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/build: core/src/erbium/liberbiumstatic.so
.PHONY : core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/build

core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/requires: core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap.c.o.requires
core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/requires: core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-engine.c.o.requires
core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/requires: core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-transactions.c.o.requires
core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/requires: core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-separate.c.o.requires
core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/requires: core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/er-coap-block1.c.o.requires
.PHONY : core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/requires

core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/clean:
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/core/src/erbium && $(CMAKE_COMMAND) -P CMakeFiles/awa_erbiumstatic_shared.dir/cmake_clean.cmake
.PHONY : core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/clean

core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/depend:
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tusnio/ci40_project/awacontiki/AwaLWM2M /home/tusnio/ci40_project/awacontiki/AwaLWM2M/core/src/erbium /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/core/src/erbium /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : core/src/erbium/CMakeFiles/awa_erbiumstatic_shared.dir/depend

