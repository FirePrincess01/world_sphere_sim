//
// @file:   main.cpp
// @author: FirePrincess
// @date:   2022-01-01
// @brief:  Program entry point
//

#include "vulkan_particle_engine/vulkan_particle_engine.h"
#include "vulkan_particle_engine/shader/simple_shader.h"
#include "vulkan_particle_engine/object/simple_object/hello_triangle.h"
#include "sphere/sphere_shader_object.h"

#include <iostream>

using namespace std;

int main()
{


    cout << "Hello World!" << endl;

    SimpleShader shader;
    HelloTriangle obj(shader);

    RenderEngine renderEngine;
	renderEngine.add(obj);

    renderEngine.run();

    return 0;
}