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

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inColor;

layout(location = 0) out vec3 position;
layout(location = 1) out vec3 fragColor;
layout(location = 2) out vec3 normal;

void main() 
{
    position = (ubo.model * vec4(inPosition, 1.0)).xyz;
    normal = inNormal;

    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);
    fragColor = inColor;
}