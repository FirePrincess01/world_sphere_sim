//
// @file:   sphere_shader_object.h
// @author: FirePrincess
// @date:   2022-01-02
// @brief:  Shader used to draw a sphere
//

#pragma once


#include "vulkan_particle_engine/shader_object/shader_object.h"
#include "vulkan_particle_engine/components/memory_mapped_buffer.h"
#include "vulkan_particle_engine/components/simple_descriptor_sets.h"
#include "vulkan_particle_engine/components/simple_descriptor_set_layout.h"
#include "vulkan_particle_engine/components/simple_descriptor_pool.h"
#include "vulkan_particle_engine/components/advanced_pipeline.h"
#include "include_glm.h"

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

	SphereShaderObject(size_t const vertexBufferSize, size_t const indexBufferSize);

	// inherited functions
    void setup(RenderEngineInterface&) final;
    void draw(RenderEngineInterface&, size_t const imageIndex) final;
    void cleanup(RenderEngineInterface&) final;

protected:
    virtual void doUpdateVertexBuffer(VertexBufferElement vertexBuffer[], size_t const size) = 0;
    virtual void doUpdateColorBuffer(ColorBufferElement colorBuffer[], size_t const size) = 0;
    virtual void doUpdateIndexBuffer(IndexBufferElement indexBuffer[], size_t const size) = 0;
    virtual void doUpdateUniformBuffer(UnformBuffer uniformBuffer[], size_t const size) = 0;

private:

	size_t const mVertexBufferSize;
	size_t const mIndexBufferSize;

    MemoryMappedBuffer<VertexBufferElement> mVertexBuffer { vk::BufferUsageFlagBits::eVertexBuffer };
    MemoryMappedBuffer<ColorBufferElement> mColorBuffer { vk::BufferUsageFlagBits::eVertexBuffer };
    MemoryMappedBuffer<IndexBufferElement> mIndexBuffer { vk::BufferUsageFlagBits::eIndexBuffer };
    MemoryMappedBuffer<UnformBuffer> mUniformBuffer { vk::BufferUsageFlagBits::eUniformBuffer };

    SimpleDescriptorSetLayout mDescriptorSetLayout;
    SimpleDescriptorPool mDescriptorPool;
    SimpleDescriptorSets mDescriptorSets;

    AdvancedGraphicsPipeline mPipeline;

	void recordCommands(RenderEngineInterface& engine);


	std::span<char const> getVertexShaderCode() const;
	std::span<char const> getGeometryShaderCode() const;
	std::span<char const> getFragmentShaderCode() const;

	vk::PrimitiveTopology getInputTopology() const;

    std::vector<vk::VertexInputAttributeDescription> getVertexAttributeDescriptions() const;
	std::vector<vk::VertexInputBindingDescription> getVertexBindingDescription() const;

	std::vector<vk::DescriptorSetLayoutBinding> getUniformBindingDescription() const;
};







