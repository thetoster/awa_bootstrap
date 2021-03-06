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
include core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/depend.make

# Include the progress variables for this target.
include core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/progress.make

# Include the compile flags for this target's objects.
include core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/flags.make

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/lwm2m_bootstrap.c.o: core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/flags.make
core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/lwm2m_bootstrap.c.o: ../core/src/bootstrap/lwm2m_bootstrap.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/lwm2m_bootstrap.c.o"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/core/src/bootstrap && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/awa_bootstrap_server_object.dir/lwm2m_bootstrap.c.o   -c /home/tusnio/ci40_project/awacontiki/AwaLWM2M/core/src/bootstrap/lwm2m_bootstrap.c

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/lwm2m_bootstrap.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/awa_bootstrap_server_object.dir/lwm2m_bootstrap.c.i"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/core/src/bootstrap && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/tusnio/ci40_project/awacontiki/AwaLWM2M/core/src/bootstrap/lwm2m_bootstrap.c > CMakeFiles/awa_bootstrap_server_object.dir/lwm2m_bootstrap.c.i

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/lwm2m_bootstrap.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/awa_bootstrap_server_object.dir/lwm2m_bootstrap.c.s"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/core/src/bootstrap && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/tusnio/ci40_project/awacontiki/AwaLWM2M/core/src/bootstrap/lwm2m_bootstrap.c -o CMakeFiles/awa_bootstrap_server_object.dir/lwm2m_bootstrap.c.s

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/lwm2m_bootstrap.c.o.requires:
.PHONY : core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/lwm2m_bootstrap.c.o.requires

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/lwm2m_bootstrap.c.o.provides: core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/lwm2m_bootstrap.c.o.requires
	$(MAKE) -f core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/build.make core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/lwm2m_bootstrap.c.o.provides.build
.PHONY : core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/lwm2m_bootstrap.c.o.provides

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/lwm2m_bootstrap.c.o.provides.build: core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/lwm2m_bootstrap.c.o

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/lwm2m_bootstrap_core.c.o: core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/flags.make
core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/lwm2m_bootstrap_core.c.o: ../core/src/bootstrap/lwm2m_bootstrap_core.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/lwm2m_bootstrap_core.c.o"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/core/src/bootstrap && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/awa_bootstrap_server_object.dir/lwm2m_bootstrap_core.c.o   -c /home/tusnio/ci40_project/awacontiki/AwaLWM2M/core/src/bootstrap/lwm2m_bootstrap_core.c

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/lwm2m_bootstrap_core.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/awa_bootstrap_server_object.dir/lwm2m_bootstrap_core.c.i"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/core/src/bootstrap && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/tusnio/ci40_project/awacontiki/AwaLWM2M/core/src/bootstrap/lwm2m_bootstrap_core.c > CMakeFiles/awa_bootstrap_server_object.dir/lwm2m_bootstrap_core.c.i

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/lwm2m_bootstrap_core.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/awa_bootstrap_server_object.dir/lwm2m_bootstrap_core.c.s"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/core/src/bootstrap && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/tusnio/ci40_project/awacontiki/AwaLWM2M/core/src/bootstrap/lwm2m_bootstrap_core.c -o CMakeFiles/awa_bootstrap_server_object.dir/lwm2m_bootstrap_core.c.s

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/lwm2m_bootstrap_core.c.o.requires:
.PHONY : core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/lwm2m_bootstrap_core.c.o.requires

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/lwm2m_bootstrap_core.c.o.provides: core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/lwm2m_bootstrap_core.c.o.requires
	$(MAKE) -f core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/build.make core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/lwm2m_bootstrap_core.c.o.provides.build
.PHONY : core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/lwm2m_bootstrap_core.c.o.provides

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/lwm2m_bootstrap_core.c.o.provides.build: core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/lwm2m_bootstrap_core.c.o

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/server/lwm2m_object_defs.c.o: core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/flags.make
core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/server/lwm2m_object_defs.c.o: ../core/src/server/lwm2m_object_defs.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/server/lwm2m_object_defs.c.o"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/core/src/bootstrap && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/awa_bootstrap_server_object.dir/__/server/lwm2m_object_defs.c.o   -c /home/tusnio/ci40_project/awacontiki/AwaLWM2M/core/src/server/lwm2m_object_defs.c

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/server/lwm2m_object_defs.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/awa_bootstrap_server_object.dir/__/server/lwm2m_object_defs.c.i"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/core/src/bootstrap && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/tusnio/ci40_project/awacontiki/AwaLWM2M/core/src/server/lwm2m_object_defs.c > CMakeFiles/awa_bootstrap_server_object.dir/__/server/lwm2m_object_defs.c.i

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/server/lwm2m_object_defs.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/awa_bootstrap_server_object.dir/__/server/lwm2m_object_defs.c.s"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/core/src/bootstrap && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/tusnio/ci40_project/awacontiki/AwaLWM2M/core/src/server/lwm2m_object_defs.c -o CMakeFiles/awa_bootstrap_server_object.dir/__/server/lwm2m_object_defs.c.s

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/server/lwm2m_object_defs.c.o.requires:
.PHONY : core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/server/lwm2m_object_defs.c.o.requires

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/server/lwm2m_object_defs.c.o.provides: core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/server/lwm2m_object_defs.c.o.requires
	$(MAKE) -f core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/build.make core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/server/lwm2m_object_defs.c.o.provides.build
.PHONY : core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/server/lwm2m_object_defs.c.o.provides

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/server/lwm2m_object_defs.c.o.provides.build: core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/server/lwm2m_object_defs.c.o

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_bootstrap_config.c.o: core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/flags.make
core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_bootstrap_config.c.o: ../core/src/common/lwm2m_bootstrap_config.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_bootstrap_config.c.o"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/core/src/bootstrap && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_bootstrap_config.c.o   -c /home/tusnio/ci40_project/awacontiki/AwaLWM2M/core/src/common/lwm2m_bootstrap_config.c

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_bootstrap_config.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_bootstrap_config.c.i"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/core/src/bootstrap && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/tusnio/ci40_project/awacontiki/AwaLWM2M/core/src/common/lwm2m_bootstrap_config.c > CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_bootstrap_config.c.i

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_bootstrap_config.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_bootstrap_config.c.s"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/core/src/bootstrap && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/tusnio/ci40_project/awacontiki/AwaLWM2M/core/src/common/lwm2m_bootstrap_config.c -o CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_bootstrap_config.c.s

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_bootstrap_config.c.o.requires:
.PHONY : core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_bootstrap_config.c.o.requires

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_bootstrap_config.c.o.provides: core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_bootstrap_config.c.o.requires
	$(MAKE) -f core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/build.make core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_bootstrap_config.c.o.provides.build
.PHONY : core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_bootstrap_config.c.o.provides

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_bootstrap_config.c.o.provides.build: core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_bootstrap_config.c.o

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_serdes.c.o: core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/flags.make
core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_serdes.c.o: ../core/src/common/lwm2m_serdes.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_serdes.c.o"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/core/src/bootstrap && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_serdes.c.o   -c /home/tusnio/ci40_project/awacontiki/AwaLWM2M/core/src/common/lwm2m_serdes.c

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_serdes.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_serdes.c.i"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/core/src/bootstrap && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/tusnio/ci40_project/awacontiki/AwaLWM2M/core/src/common/lwm2m_serdes.c > CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_serdes.c.i

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_serdes.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_serdes.c.s"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/core/src/bootstrap && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/tusnio/ci40_project/awacontiki/AwaLWM2M/core/src/common/lwm2m_serdes.c -o CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_serdes.c.s

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_serdes.c.o.requires:
.PHONY : core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_serdes.c.o.requires

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_serdes.c.o.provides: core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_serdes.c.o.requires
	$(MAKE) -f core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/build.make core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_serdes.c.o.provides.build
.PHONY : core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_serdes.c.o.provides

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_serdes.c.o.provides.build: core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_serdes.c.o

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_tlv.c.o: core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/flags.make
core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_tlv.c.o: ../core/src/common/lwm2m_tlv.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/CMakeFiles $(CMAKE_PROGRESS_6)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_tlv.c.o"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/core/src/bootstrap && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_tlv.c.o   -c /home/tusnio/ci40_project/awacontiki/AwaLWM2M/core/src/common/lwm2m_tlv.c

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_tlv.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_tlv.c.i"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/core/src/bootstrap && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/tusnio/ci40_project/awacontiki/AwaLWM2M/core/src/common/lwm2m_tlv.c > CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_tlv.c.i

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_tlv.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_tlv.c.s"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/core/src/bootstrap && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/tusnio/ci40_project/awacontiki/AwaLWM2M/core/src/common/lwm2m_tlv.c -o CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_tlv.c.s

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_tlv.c.o.requires:
.PHONY : core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_tlv.c.o.requires

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_tlv.c.o.provides: core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_tlv.c.o.requires
	$(MAKE) -f core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/build.make core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_tlv.c.o.provides.build
.PHONY : core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_tlv.c.o.provides

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_tlv.c.o.provides.build: core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_tlv.c.o

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_plaintext.c.o: core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/flags.make
core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_plaintext.c.o: ../core/src/common/lwm2m_plaintext.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/CMakeFiles $(CMAKE_PROGRESS_7)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_plaintext.c.o"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/core/src/bootstrap && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_plaintext.c.o   -c /home/tusnio/ci40_project/awacontiki/AwaLWM2M/core/src/common/lwm2m_plaintext.c

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_plaintext.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_plaintext.c.i"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/core/src/bootstrap && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/tusnio/ci40_project/awacontiki/AwaLWM2M/core/src/common/lwm2m_plaintext.c > CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_plaintext.c.i

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_plaintext.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_plaintext.c.s"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/core/src/bootstrap && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/tusnio/ci40_project/awacontiki/AwaLWM2M/core/src/common/lwm2m_plaintext.c -o CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_plaintext.c.s

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_plaintext.c.o.requires:
.PHONY : core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_plaintext.c.o.requires

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_plaintext.c.o.provides: core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_plaintext.c.o.requires
	$(MAKE) -f core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/build.make core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_plaintext.c.o.provides.build
.PHONY : core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_plaintext.c.o.provides

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_plaintext.c.o.provides.build: core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_plaintext.c.o

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_prettyprint.c.o: core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/flags.make
core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_prettyprint.c.o: ../core/src/common/lwm2m_prettyprint.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/CMakeFiles $(CMAKE_PROGRESS_8)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_prettyprint.c.o"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/core/src/bootstrap && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_prettyprint.c.o   -c /home/tusnio/ci40_project/awacontiki/AwaLWM2M/core/src/common/lwm2m_prettyprint.c

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_prettyprint.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_prettyprint.c.i"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/core/src/bootstrap && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/tusnio/ci40_project/awacontiki/AwaLWM2M/core/src/common/lwm2m_prettyprint.c > CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_prettyprint.c.i

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_prettyprint.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_prettyprint.c.s"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/core/src/bootstrap && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/tusnio/ci40_project/awacontiki/AwaLWM2M/core/src/common/lwm2m_prettyprint.c -o CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_prettyprint.c.s

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_prettyprint.c.o.requires:
.PHONY : core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_prettyprint.c.o.requires

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_prettyprint.c.o.provides: core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_prettyprint.c.o.requires
	$(MAKE) -f core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/build.make core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_prettyprint.c.o.provides.build
.PHONY : core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_prettyprint.c.o.provides

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_prettyprint.c.o.provides.build: core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_prettyprint.c.o

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_opaque.c.o: core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/flags.make
core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_opaque.c.o: ../core/src/common/lwm2m_opaque.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/CMakeFiles $(CMAKE_PROGRESS_9)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_opaque.c.o"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/core/src/bootstrap && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_opaque.c.o   -c /home/tusnio/ci40_project/awacontiki/AwaLWM2M/core/src/common/lwm2m_opaque.c

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_opaque.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_opaque.c.i"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/core/src/bootstrap && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/tusnio/ci40_project/awacontiki/AwaLWM2M/core/src/common/lwm2m_opaque.c > CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_opaque.c.i

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_opaque.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_opaque.c.s"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/core/src/bootstrap && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/tusnio/ci40_project/awacontiki/AwaLWM2M/core/src/common/lwm2m_opaque.c -o CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_opaque.c.s

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_opaque.c.o.requires:
.PHONY : core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_opaque.c.o.requires

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_opaque.c.o.provides: core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_opaque.c.o.requires
	$(MAKE) -f core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/build.make core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_opaque.c.o.provides.build
.PHONY : core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_opaque.c.o.provides

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_opaque.c.o.provides.build: core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_opaque.c.o

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_tree_builder.c.o: core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/flags.make
core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_tree_builder.c.o: ../core/src/common/lwm2m_tree_builder.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/CMakeFiles $(CMAKE_PROGRESS_10)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_tree_builder.c.o"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/core/src/bootstrap && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_tree_builder.c.o   -c /home/tusnio/ci40_project/awacontiki/AwaLWM2M/core/src/common/lwm2m_tree_builder.c

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_tree_builder.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_tree_builder.c.i"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/core/src/bootstrap && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/tusnio/ci40_project/awacontiki/AwaLWM2M/core/src/common/lwm2m_tree_builder.c > CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_tree_builder.c.i

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_tree_builder.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_tree_builder.c.s"
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/core/src/bootstrap && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/tusnio/ci40_project/awacontiki/AwaLWM2M/core/src/common/lwm2m_tree_builder.c -o CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_tree_builder.c.s

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_tree_builder.c.o.requires:
.PHONY : core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_tree_builder.c.o.requires

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_tree_builder.c.o.provides: core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_tree_builder.c.o.requires
	$(MAKE) -f core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/build.make core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_tree_builder.c.o.provides.build
.PHONY : core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_tree_builder.c.o.provides

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_tree_builder.c.o.provides.build: core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_tree_builder.c.o

awa_bootstrap_server_object: core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/lwm2m_bootstrap.c.o
awa_bootstrap_server_object: core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/lwm2m_bootstrap_core.c.o
awa_bootstrap_server_object: core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/server/lwm2m_object_defs.c.o
awa_bootstrap_server_object: core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_bootstrap_config.c.o
awa_bootstrap_server_object: core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_serdes.c.o
awa_bootstrap_server_object: core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_tlv.c.o
awa_bootstrap_server_object: core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_plaintext.c.o
awa_bootstrap_server_object: core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_prettyprint.c.o
awa_bootstrap_server_object: core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_opaque.c.o
awa_bootstrap_server_object: core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_tree_builder.c.o
awa_bootstrap_server_object: core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/build.make
.PHONY : awa_bootstrap_server_object

# Rule to build all files generated by this target.
core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/build: awa_bootstrap_server_object
.PHONY : core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/build

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/requires: core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/lwm2m_bootstrap.c.o.requires
core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/requires: core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/lwm2m_bootstrap_core.c.o.requires
core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/requires: core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/server/lwm2m_object_defs.c.o.requires
core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/requires: core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_bootstrap_config.c.o.requires
core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/requires: core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_serdes.c.o.requires
core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/requires: core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_tlv.c.o.requires
core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/requires: core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_plaintext.c.o.requires
core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/requires: core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_prettyprint.c.o.requires
core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/requires: core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_opaque.c.o.requires
core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/requires: core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/__/common/lwm2m_tree_builder.c.o.requires
.PHONY : core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/requires

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/clean:
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/core/src/bootstrap && $(CMAKE_COMMAND) -P CMakeFiles/awa_bootstrap_server_object.dir/cmake_clean.cmake
.PHONY : core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/clean

core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/depend:
	cd /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tusnio/ci40_project/awacontiki/AwaLWM2M /home/tusnio/ci40_project/awacontiki/AwaLWM2M/core/src/bootstrap /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/core/src/bootstrap /home/tusnio/ci40_project/awacontiki/AwaLWM2M/build/core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : core/src/bootstrap/CMakeFiles/awa_bootstrap_server_object.dir/depend

