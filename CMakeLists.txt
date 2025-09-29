# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Set the project name
project(LibraryManagementSystem)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find the SQLite3 library
find_package(SQLite3 REQUIRED)

# Add the executable target from the source files
add_executable(library_backend
    main.cpp
    LibraryDB.cpp
    EmailService.cpp
)

# Link the SQLite3 library to our executable
target_link_libraries(library_backend PRIVATE SQLite::SQLite3)

# Optional: Enable compiler warnings for better code
if(MSVC)
    # Microsoft Visual C++ Compiler
    target_compile_options(library_backend PRIVATE /W4)
else()
    # GCC/Clang
    target_compile_options(library_backend PRIVATE -Wall -Wextra -Wpedantic)
endif()