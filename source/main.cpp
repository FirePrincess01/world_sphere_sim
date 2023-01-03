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
#include "geometry/cube.h"
#include "geometry/sphere.h"

#include <iostream>

using namespace std;


class Cube
{
public:
    Cube(glm::vec3 const & pos, glm::vec3 const & color,
        glm::mat4 const & view, glm::mat4 const & proj)
    : mPos(pos),
      mView(view),
      mProj(proj)
    {
        mShaderObject.initVertexBuffer.set<&Cube::initVertexData>(*this);
        mShaderObject.updateColorBuffer.set<&Cube::updateColorData2>(*this);
        mShaderObject.updateUniformBuffer.set<&Cube::updateUniformData>(*this);
    }

    SphereShaderObject& get() {
        return mShaderObject;
    }

    void initVertexData(std::span<SphereShaderObject::VertexBufferElement> data)
    {
        assert(data.size() == cube_vertices.size());
        for(size_t i = 0; i < data.size(); ++i)
        {
            data[i].pos = cube_vertices[i];
        }
    }

    void updateColorData2(std::span<SphereShaderObject::ColorBufferElement> data)
    {
        assert(data.size() == cube_colors2.size());
        for(size_t i = 0; i < data.size(); ++i)
        {
            data[i].r = cube_colors2[i].r;
            data[i].g = cube_colors2[i].g;
            data[i].b = cube_colors2[i].b;
        }
    }

    void updateUniformData(std::span<SphereShaderObject::UnformBuffer> data) 
    {
        assert(data.size() == 1);

        data[0].model =  glm::translate(glm::mat4(1), mPos);
        data[0].model = glm::rotate(data[0].model, count++ * 0.00003f, {0.5f, 0.5f, 0.0f});
        data[0].view = mView;
        data[0].proj = mProj;
		data[0].lightPosition = glm::vec3(10.0f, 10.0f, 10.0f);
		data[0].ambient = 0.2f;
    };

private:
    std::vector<AdvancedShader::VertexBufferElement> const vertexData;

    // std::array<glm::vec3, 36> const cube_vertices = createCubeTriangles();
    std::vector<glm::vec3> const cube_vertices = flatSphereData(createSphereVertices(2.0f, 10));

    std::array<glm::vec3, 6>  const cube_colors2 = {
        glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(0.1f, 0.5f, 0.5f),
        glm::vec3(0.5f, 0.1f, 0.5f),
        glm::vec3(0.5f, 0.5f, 0.1f),
        glm::vec3(0.5f, 0.1f, 0.1f),
        glm::vec3(0.1f, 0.1f, 0.5f)
    };

    SphereShaderObject mShaderObject = SphereShaderObject(cube_vertices.size(), cube_colors2.size());

    glm::vec3 mPos;

    glm::mat4 const & mView;
    glm::mat4 const & mProj;

    uint64_t count = 0;
};


int main()
{
    cout << "#######------- World Sphere Sim -------#######" << endl;

    glm::mat4 view = glm::mat4(1);
    glm::mat4 proj = glm::mat4(1);

    Cube cube = Cube({0.0f, 0.0f, 1.2f}, {0.4f, 0.7f, 0.1f}, view, proj);

    SimpleShader shader;
    HelloTriangle obj(shader);

    RenderEngine renderEngine;
	renderEngine.add(obj);
	renderEngine.add(cube.get());


    uint64_t count = 0;
    auto lbdStartOfNextFrame = [&](){
        glm::vec3 posEye =  {0.0f, -8.0f, 0.0f};
        glm::vec3 posView = {0.0f, 0.0f, 0.0f};

        view = glm::lookAt(posEye, posView, glm::vec3(0.0f, 0.0f, 1.0f));
        view = glm::rotate(view, count++ * 0.0003f, {0.0f, 0.0f, 1.0f});
        proj = glm::perspective(glm::radians(45.0f), 
            static_cast<float>(renderEngine.getSwapChainExtent().width) / static_cast<float>(renderEngine.getSwapChainExtent().height), 
            0.1f, 100'000.0f);
        proj[1][1] *= -1; // invert Y for Vulkan
    };

    renderEngine.startOfNextFrame.add(lbdStartOfNextFrame);
    renderEngine.run();

    return 0;
}