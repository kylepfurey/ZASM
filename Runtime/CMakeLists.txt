# .txt
# ZASM CMake Configuration
# by Kyle Furey

# Initialize CMake with vcpkg (if Windows)
#set(CMAKE_TOOLCHAIN_FILE "$ENV{USERPROFILE}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE FILEPATH "vcpkg Toolchain File")
cmake_minimum_required(VERSION 3.16)
project(libZASM C)

# Set C compiler version
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/lib)

# Build ZASM Runtime Library
add_library(libZASM SHARED
        ${CMAKE_SOURCE_DIR}/include/ZASM.h
        ${CMAKE_SOURCE_DIR}/include/External.h
        ${CMAKE_SOURCE_DIR}/include/Export.h
        ${CMAKE_SOURCE_DIR}/include/Types.h
        ${CMAKE_SOURCE_DIR}/include/Error.h
        ${CMAKE_SOURCE_DIR}/include/Execute.h
        ${CMAKE_SOURCE_DIR}/include/Program.h
        ${CMAKE_SOURCE_DIR}/include/Vector.h
        ${CMAKE_SOURCE_DIR}/include/Coroutine.h
        ${CMAKE_SOURCE_DIR}/include/FileStream.h
        ${CMAKE_SOURCE_DIR}/include/Stack.h
        ${CMAKE_SOURCE_DIR}/include/Opcode.h
        ${CMAKE_SOURCE_DIR}/include/Library.h
        ${CMAKE_SOURCE_DIR}/src/Execute.c
        ${CMAKE_SOURCE_DIR}/src/Program.c
        ${CMAKE_SOURCE_DIR}/src/Vector.c
        ${CMAKE_SOURCE_DIR}/src/Coroutine.c
        ${CMAKE_SOURCE_DIR}/src/FileStream.c
        ${CMAKE_SOURCE_DIR}/src/Stack.c
        ${CMAKE_SOURCE_DIR}/src/Opcode.c
        ${CMAKE_SOURCE_DIR}/src/Library.c
)

# Rename the ZASM Runtime Library
set_target_properties(libZASM PROPERTIES OUTPUT_NAME "ZASM")

# Set include directory and build macro
target_include_directories(libZASM PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_compile_definitions(libZASM PUBLIC
        $<$<CONFIG:Debug>:_DEBUG>
)
target_compile_definitions(libZASM PRIVATE
        ZASM_EXPORTS
)

# Find external libraries
find_package(PkgConfig REQUIRED)                # Linux
pkg_check_modules(LIBFFI REQUIRED libffi)       # Linux

# Include external libraries
target_include_directories(libZASM PUBLIC
        ${LIBFFI_INCLUDE_DIRS}
)

# Link external libraries to ZASM Runtime Library
target_link_libraries(libZASM PUBLIC
        #libffi::libffi         # Windows
        ${LIBFFI_LIBRARIES}     # Linux
)

# Build ZASM Runtime Executable
add_executable(ZASM
        ${CMAKE_SOURCE_DIR}/include/ZASM.h
        ${CMAKE_SOURCE_DIR}/include/External.h
        ${CMAKE_SOURCE_DIR}/include/Export.h
        ${CMAKE_SOURCE_DIR}/include/Types.h
        ${CMAKE_SOURCE_DIR}/include/Error.h
        ${CMAKE_SOURCE_DIR}/include/Execute.h
        ${CMAKE_SOURCE_DIR}/include/Program.h
        ${CMAKE_SOURCE_DIR}/include/Vector.h
        ${CMAKE_SOURCE_DIR}/include/Coroutine.h
        ${CMAKE_SOURCE_DIR}/include/FileStream.h
        ${CMAKE_SOURCE_DIR}/include/Stack.h
        ${CMAKE_SOURCE_DIR}/include/Opcode.h
        ${CMAKE_SOURCE_DIR}/include/Library.h
        ${CMAKE_SOURCE_DIR}/src/Execute.c
        ${CMAKE_SOURCE_DIR}/src/Program.c
        ${CMAKE_SOURCE_DIR}/src/Vector.c
        ${CMAKE_SOURCE_DIR}/src/Coroutine.c
        ${CMAKE_SOURCE_DIR}/src/FileStream.c
        ${CMAKE_SOURCE_DIR}/src/Stack.c
        ${CMAKE_SOURCE_DIR}/src/Opcode.c
        ${CMAKE_SOURCE_DIR}/src/Library.c
        ${CMAKE_SOURCE_DIR}/main.c
)

# Set include directory and build macro
target_include_directories(ZASM PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_compile_definitions(ZASM PRIVATE
        $<$<CONFIG:Debug>:_DEBUG>
)

# Rename the ZASM Runtime Executable
set_target_properties(ZASM PROPERTIES OUTPUT_NAME "Z")

# Include external libraries
target_include_directories(ZASM PRIVATE
        ${LIBFFI_INCLUDE_DIRS}
)

# Link external libraries to ZASM Runtime Executable
target_link_libraries(ZASM PRIVATE
        #libffi::libffi         # Windows
        ${LIBFFI_LIBRARIES}     # Linux
)
