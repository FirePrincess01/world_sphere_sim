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


# compile glsl
find_package(Vulkan REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC Vulkan::Vulkan)
add_executable(embedfile "${CMAKE_SOURCE_DIR}/embedfile/embedfile.cpp")

file(GLOB SHADER_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/shaders/sphere/*.frag"
    "${CMAKE_SOURCE_DIR}/shaders/sphere/*.vert"
    "${CMAKE_SOURCE_DIR}/shaders/sphere/*.geom"
)

foreach(SHADER_SOURCE ${SHADER_SOURCES})
    get_filename_component(file_c ${SHADER_SOURCE} NAME)
    string(REPLACE "." "_" file_c_2 ${file_c})

    set(SHADER_SPV ${CMAKE_BINARY_DIR}/shaders/${file_c_2}.spv)
    set(SHADER_HEADER ${CMAKE_BINARY_DIR}/shaders/${file_c_2}.h)

    add_custom_command(
        OUTPUT ${SHADER_HEADER}
        DEPENDS ${SHADER_SOURCE}
        COMMAND Vulkan::glslc ${SHADER_SOURCE} -O -o ${SHADER_SPV}
        COMMAND embedfile ${file_c_2} ${SHADER_SPV} ${SHADER_HEADER}
    )
    target_sources(${PROJECT_NAME} PRIVATE ${SHADER_HEADER})
endforeach()

target_include_directories(${PROJECT_NAME} PRIVATE  
    ${CMAKE_BINARY_DIR}/shaders/
)


# include subprojects
add_subdirectory(vulkan_particle_engine)
target_link_libraries(${PROJECT_NAME} PRIVATE vulkan_particle_engine)