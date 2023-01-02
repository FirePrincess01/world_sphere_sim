//
// @file:   cube.h
// @author: FirePrincess
// @date:   2022-01-01
// @brief:  Creates a cube
//

#pragma once

#include "include_glm.h"

// Vulkan Coordinate System
//  z
//	 /|    
//   |  __ y
//   |   /| 
//   |  /
//   | /
//   |_____________> x
//
//

//
//   C ----------------- B
//    /                /|
//   / |              / |
// D/  |           A /  |
// -----------------/   |
// |   |            |   |
// |   |            |   |
// |  G ------------|-- | F
// |  /             |   /
// | /              |  /
// |/               | /
// ------------------
// H                E


auto createCubeVertices(float const a = 0.5f)
{
    const glm::vec3 A = glm::vec3(  a, -a,  a );    
    const glm::vec3 B = glm::vec3(  a,  a,  a );   
    const glm::vec3 C = glm::vec3( -a,  a,  a ); 
    const glm::vec3 D = glm::vec3( -a, -a,  a );     

    const glm::vec3 E = glm::vec3(  a, -a, -a );  
    const glm::vec3 F = glm::vec3(  a,  a, -a );    
    const glm::vec3 G = glm::vec3( -a,  a, -a );   
    const glm::vec3 H = glm::vec3( -a, -a, -a );   

    constexpr uint32_t size = 8;

    const std::array<glm::vec3, size> cube = 
    {
        A, B, C, D, E, F, G, H
    };

    return cube;
}

auto createCubeIndices()
{
    uint32_t const A = 0;
    uint32_t const B = 1;
    uint32_t const C = 2;
    uint32_t const D = 3;
    uint32_t const E = 4;
    uint32_t const F = 5;
    uint32_t const G = 6;
    uint32_t const H = 7;

    constexpr uint32_t faces = 6;
    constexpr uint32_t triangle_per_face = 2;
    constexpr uint32_t vertex_per_triangle = 3;
    constexpr uint32_t size = faces * triangle_per_face * vertex_per_triangle;

    const std::array<uint32_t, size> data = 
    {   
        // top
        A, B, C,
        C, D, A,

        // bottom
        G, F, E,
        E, H, G,

        // left
        H, D, C,
        C, G, H,

        // right
        A, E, F,
        F, B, A,

        // front
        E, A, D,
        D, H, E,

        // back
        B, F, G,
        G, C, B
    };

    return data;
}

