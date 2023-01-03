//
// @file:   sphere.h
// @author: FirePrincess
// @date:   2022-01-03
// @brief:  Creates a sphere
//

#pragma once

#include "include_glm.h"
#include <glm/gtc/constants.hpp>

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

auto createSphereVertices(float const r = 0.5, size_t const n = 10)
{
    std::vector<std::vector<glm::vec3>> data;
    data.resize(n);
    for(auto & elem : data) {
        elem.resize(n);
    }

    for(size_t j = 0; j < n; ++j) {
        
        float const beta = ((glm::pi<float>()) / (n-1)) * j;

        float const z = glm::cos(beta) * r;
        float const sub_r = glm::abs(glm::sin(beta) * r);

        for(size_t i = 0; i < n; ++i) {
            float const alpha = (2 * (glm::pi<float>()) / n) * i;

            float const x = glm::cos(alpha) * sub_r;
            float const y = glm::sin(alpha) * sub_r;

            data[j][i] = glm::vec3(x, y, z);
        }
    }

    return data;
}

auto createPlaneVertices(float const r = 0.5, size_t const n = 10)
{
    std::vector<std::vector<glm::vec3>> data;
    data.resize(n);
    for(auto & elem : data) {
        elem.resize(n);
    }

    float const a = r * 2;

    for(size_t j = 0; j < n; ++j) {
        
        float const z = 0;

        for(size_t i = 0; i < n; ++i) {

            float const x = -a + a / n * i;
            float const y = -a + a / n * j;

            data[j][i] = glm::vec3(x, y, z);
        }
    }

    return data;
}

std::vector<glm::vec3> flatSphereData(std::vector<std::vector<glm::vec3>> const & data)
{
    std::vector<glm::vec3> res;
    for(auto & elem : data) {
        for(auto & elem2 : elem) {
            res.push_back(elem2);
        }
    }

    return res;
}

std::vector<glm::vec3> createSphereTriangles(std::vector<std::vector<glm::vec3>> const & data)
{
    size_t const n = data.size();
    for(auto & elem : data) {
        assert(elem.size() == n);
    }

    std::vector<glm::vec3> res;

    for(size_t j = 0; j < n; ++j) {
        for(size_t i = 0; i < n; ++i) {
            res.push_back(data[j%n][i%n]);
            res.push_back(data[(j+1)%n][i%n]);
            res.push_back(data[j%n][(i+1)%n]);

            res.push_back(data[(j+1)%n][(i+1)%n]);
            res.push_back(data[(j)%n][(i+1)%n]);
            res.push_back(data[(j+1)%n][(i)%n]);
        }
    }

    return res;
}