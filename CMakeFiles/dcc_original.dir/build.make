# CMAKE generated file: DO NOT EDIT!
# Generated by "MSYS Makefiles" Generator, CMake Version 3.15

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
CMAKE_COMMAND = /C/msys64/mingw64/bin/cmake.exe

# The command to remove a file.
RM = /C/msys64/mingw64/bin/cmake.exe -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /D/src/reverse_engineering/jopadan/dcc

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /D/src/reverse_engineering/jopadan/dcc

# Include any dependencies generated for this target.
include CMakeFiles/dcc_original.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/dcc_original.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/dcc_original.dir/flags.make

CMakeFiles/dcc_original.dir/dcc_original_autogen/mocs_compilation.cpp.obj: CMakeFiles/dcc_original.dir/flags.make
CMakeFiles/dcc_original.dir/dcc_original_autogen/mocs_compilation.cpp.obj: dcc_original_autogen/mocs_compilation.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/D/src/reverse_engineering/jopadan/dcc/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/dcc_original.dir/dcc_original_autogen/mocs_compilation.cpp.obj"
	/C/msys64/mingw64/bin/g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/dcc_original.dir/dcc_original_autogen/mocs_compilation.cpp.obj -c /D/src/reverse_engineering/jopadan/dcc/dcc_original_autogen/mocs_compilation.cpp

CMakeFiles/dcc_original.dir/dcc_original_autogen/mocs_compilation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/dcc_original.dir/dcc_original_autogen/mocs_compilation.cpp.i"
	/C/msys64/mingw64/bin/g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /D/src/reverse_engineering/jopadan/dcc/dcc_original_autogen/mocs_compilation.cpp > CMakeFiles/dcc_original.dir/dcc_original_autogen/mocs_compilation.cpp.i

CMakeFiles/dcc_original.dir/dcc_original_autogen/mocs_compilation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/dcc_original.dir/dcc_original_autogen/mocs_compilation.cpp.s"
	/C/msys64/mingw64/bin/g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /D/src/reverse_engineering/jopadan/dcc/dcc_original_autogen/mocs_compilation.cpp -o CMakeFiles/dcc_original.dir/dcc_original_autogen/mocs_compilation.cpp.s

CMakeFiles/dcc_original.dir/src/dcc.cpp.obj: CMakeFiles/dcc_original.dir/flags.make
CMakeFiles/dcc_original.dir/src/dcc.cpp.obj: src/dcc.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/D/src/reverse_engineering/jopadan/dcc/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/dcc_original.dir/src/dcc.cpp.obj"
	/C/msys64/mingw64/bin/g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/dcc_original.dir/src/dcc.cpp.obj -c /D/src/reverse_engineering/jopadan/dcc/src/dcc.cpp

CMakeFiles/dcc_original.dir/src/dcc.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/dcc_original.dir/src/dcc.cpp.i"
	/C/msys64/mingw64/bin/g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /D/src/reverse_engineering/jopadan/dcc/src/dcc.cpp > CMakeFiles/dcc_original.dir/src/dcc.cpp.i

CMakeFiles/dcc_original.dir/src/dcc.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/dcc_original.dir/src/dcc.cpp.s"
	/C/msys64/mingw64/bin/g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /D/src/reverse_engineering/jopadan/dcc/src/dcc.cpp -o CMakeFiles/dcc_original.dir/src/dcc.cpp.s

# Object files for target dcc_original
dcc_original_OBJECTS = \
"CMakeFiles/dcc_original.dir/dcc_original_autogen/mocs_compilation.cpp.obj" \
"CMakeFiles/dcc_original.dir/src/dcc.cpp.obj"

# External object files for target dcc_original
dcc_original_EXTERNAL_OBJECTS =

dcc_original.exe: CMakeFiles/dcc_original.dir/dcc_original_autogen/mocs_compilation.cpp.obj
dcc_original.exe: CMakeFiles/dcc_original.dir/src/dcc.cpp.obj
dcc_original.exe: CMakeFiles/dcc_original.dir/build.make
dcc_original.exe: libdcc_lib.a
dcc_original.exe: common/libdcc_hash.a
dcc_original.exe: 3rd_party/libdisasm/libdisasm_s.a
dcc_original.exe: C:/msys64/mingw64/lib/libQt5Core.dll.a
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/D/src/reverse_engineering/jopadan/dcc/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable dcc_original.exe"
	/C/msys64/mingw64/bin/cmake.exe -E remove -f CMakeFiles/dcc_original.dir/objects.a
	/C/msys64/mingw64/bin/ar.exe cr CMakeFiles/dcc_original.dir/objects.a $(dcc_original_OBJECTS) $(dcc_original_EXTERNAL_OBJECTS)
	/C/msys64/mingw64/bin/g++.exe  -Wall -std=c++11   -Wl,--whole-archive CMakeFiles/dcc_original.dir/objects.a -Wl,--no-whole-archive  -o dcc_original.exe -Wl,--major-image-version,0,--minor-image-version,0 libdcc_lib.a common/libdcc_hash.a 3rd_party/libdisasm/libdisasm_s.a /C/msys64/mingw64/lib/libQt5Core.dll.a -lkernel32 -luser32 -lgdi32 -lwinspool -lshell32 -lole32 -loleaut32 -luuid -lcomdlg32 -ladvapi32 

# Rule to build all files generated by this target.
CMakeFiles/dcc_original.dir/build: dcc_original.exe

.PHONY : CMakeFiles/dcc_original.dir/build

CMakeFiles/dcc_original.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/dcc_original.dir/cmake_clean.cmake
.PHONY : CMakeFiles/dcc_original.dir/clean

CMakeFiles/dcc_original.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MSYS Makefiles" /D/src/reverse_engineering/jopadan/dcc /D/src/reverse_engineering/jopadan/dcc /D/src/reverse_engineering/jopadan/dcc /D/src/reverse_engineering/jopadan/dcc /D/src/reverse_engineering/jopadan/dcc/CMakeFiles/dcc_original.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/dcc_original.dir/depend

