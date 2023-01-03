//
// @file:   spher_shader.frag
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

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 fragColor;
layout(location = 2) in vec3 normal;

layout(location = 0) out vec4 outColor;

void main() {
    // vec3 normal = normalize(cross(dFdx(position), dFdy(position)));
    vec3 lightDir = normalize(-ubo.lightPosition);

    float strenght = min(1.0, ubo.ambient + max(0,dot(lightDir, normal)));

    // outColor = vec4(fragColor * strenght, 0.1);

    outColor = vec4(fragColor, 0.1);
}