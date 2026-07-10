cmake_minimum_required(VERSION 3.14)
project(NeepuSTU_Cli)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include FetchContent for downloading dependencies
include(FetchContent)

# Fetch CPR library (libcurl wrapper)
FetchContent_Declare(
    cpr
    GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG master
)

# Configure CPR options
set(CPR_FORCE_USE_SYSTEM_CURL OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(CPR_CURL_USE_LIBPSL OFF CACHE BOOL "" FORCE)

# Make CPR available
FetchContent_MakeAvailable(cpr)

# Define the executable
add_executable(NeepuSTU_Cli NeepuCli.cpp)

# Link CPR to the executable
target_link_libraries(NeepuSTU_Cli PRIVATE cpr::cpr)

# Set input charset for MSVC
if(MSVC)
    target_compile_options(NeepuSTU_Cli PRIVATE /source-charset:utf-8)
endif()

# For GCC/Clang, set input charset
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(NeepuSTU_Cli PRIVATE -finput-charset=UTF-8)
endif()
