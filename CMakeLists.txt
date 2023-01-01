#
# \file       CMakeLists.txt
# \author     FirePrincess
# \date       2023-01-01
#

cmake_minimum_required(VERSION 3.20)

# Create the project
project("world_sphere_sim" CXX)

# Export compile commands for VSCode C++ Intellisense
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(
    # -fdiagnostics-color
    # -Wall 
)

# add executable
add_executable(${PROJECT_NAME})

# include directories
target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_SOURCE_DIR}/source"
)

# add source files
file(GLOB SOURCE_FILES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/source/*.cpp"
    "${CMAKE_SOURCE_DIR}/source/*/*.cpp"
)

target_sources(${PROJECT_NAME} PRIVATE ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE vulkan_particle_engine)

add_subdirectory(vulkan_particle_engine)