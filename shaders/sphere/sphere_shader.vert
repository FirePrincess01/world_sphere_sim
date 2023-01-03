//
// @file:   spher_shader.vert
// @author: FirePrincess
// @date:   2022-01-02
// @brief:  Shader to draw a mesh of triangles
//

#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject
{
    mat4 model;
    mat4 view;
    mat4 proj;
    vec3 lightPosition;
    float ambient;
} ubo;

struct StorageBufferElement
{
    float r;
    float g;
    float b;
};

layout(std430, binding = 1) readonly buffer StorageBufferObject
{
    StorageBufferElement elems[];
} sbo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;

layout(location = 0) out vec3 position;
layout(location = 1) out vec3 fragColor;
layout(location = 2) out vec3 normal;

void main() 
{
    uint i = gl_VertexIndex;

    position = (ubo.model * vec4(inPosition, 1.0)).xyz;
    normal = inNormal;

    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);
    // fragColor = inColor;

    uint color_index = min(i/6, 6);
    fragColor = vec3(sbo.elems[color_index].r, sbo.elems[color_index].g, sbo.elems[color_index].b);
}