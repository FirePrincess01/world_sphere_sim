//
// @file:   sphere_shader_object.h
// @author: FirePrincess
// @date:   2022-01-02
// @brief:  Shader used to draw a sphere
//

#pragma once


#include "vulkan_particle_engine/shader_object/shader_object.h"
#include "vulkan_particle_engine/components/memory_mapped_buffer.h"

class SphereShaderObject : public ShaderObject
{
public:

	struct VertexBufferElement {
		glm::vec3 pos;
		glm::vec3 normal;
	};

	struct ColorBufferElement {
		glm::vec3 color;
	};

	struct IndexBufferElement {
		uint32_t index;
	};

	struct UnformBuffer {
		alignas(16) glm::mat4 model;
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 proj;
		alignas(16) glm::vec3 lightPosition;
		alignas(4)  float ambient;
	};

	// inherited functions
    void setup(RenderEngineInterface&) final;
    void draw(RenderEngineInterface&, size_t const imageIndex) final;
    void cleanup(RenderEngineInterface&) final;

protected:
    virtual void doUpdateVertexBuffer(VertexBufferElement vertexBuffer[], size_t const size) = 0;
    virtual void doUpdateColorBuffer(ColorBufferElement colorBuffer[], size_t const size) = 0;
    virtual void doUpdateIndexBuffer(IndexBufferElement indexBuffer[], size_t const size) = 0;
    virtual void doUpdateUniformBuffer(UnformBuffer uniformBuffer[]) = 0;

private:

    MemoryMappedBuffer<VertexBufferElement> mVertexBuffer { vk::BufferUsageFlagBits::eVertexBuffer };
    MemoryMappedBuffer<ColorBufferElement> mColorBuffer { vk::BufferUsageFlagBits::eVertexBuffer };
    MemoryMappedBuffer<IndexBufferElement> mIndexBuffer { vk::BufferUsageFlagBits::eIndexBuffer };
    MemoryMappedBuffer<UnformBuffer> mUniformBuffer { vk::BufferUsageFlagBits::eUniformBuffer };

    SimpleDescriptorSetLayout mDescriptorSetLayout;
    SimpleDescriptorPool mDescriptorPool;
    SimpleDescriptorSets mDescriptorSets;

    AdvancedGraphicsPipeline mPipeline;
    AdvancedCommandBuffer mCommands;


	std::vector<char> getVertexShaderCode() const;
	std::vector<char> getFragmentShaderCode() const;

	vk::PrimitiveTopology getInputTopology() const;

    std::vector<vk::VertexInputAttributeDescription> getVertexAttributeDescriptions() const;
	vk::VertexInputBindingDescription getVertexBindingDescription() const;

	std::vector<vk::DescriptorSetLayoutBinding> getUniformBindingDescription() const;
};







