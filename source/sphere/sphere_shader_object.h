//
// @file:   sphere_shader_object.h
// @author: FirePrincess
// @date:   2022-01-02
// @brief:  Shader used to draw a sphere
//

#pragma once


#include "vulkan_particle_engine/shader_object/shader_object.h"
#include "vulkan_particle_engine/components/memory_mapped_buffer.h"
#include "vulkan_particle_engine/components/advanced_descriptor_sets.h"
#include "vulkan_particle_engine/components/simple_descriptor_set_layout.h"
#include "vulkan_particle_engine/components/advanced_descriptor_pool.h"
#include "vulkan_particle_engine/components/advanced_pipeline.h"
#include "include_glm.h"

class SphereShaderObject : public ShaderObject
{
public:

	struct VertexBufferElement {
		glm::vec3 pos;
		glm::vec3 normal;
	};

	struct ColorBufferElement2 {
		// glm::vec3 color;
		alignas(4) float r;
		alignas(4) float g;
		alignas(4) float b;
	};

	struct UnformBuffer {
		alignas(16) glm::mat4 model;
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 proj;
		alignas(16) glm::vec3 lightPosition;
		alignas(4)  float ambient;
	};

	Delegate<void(std::span<VertexBufferElement>)> updateVertexBuffer;
	Delegate<void(std::span<ColorBufferElement2>)>  updateColorBuffer2;
	Delegate<void(std::span<UnformBuffer>)> updateUniformBuffer;

	Delegate<void(std::span<VertexBufferElement>)> initVertexBuffer;
	Delegate<void(std::span<ColorBufferElement2>)>  initColorBuffer2;

	SphereShaderObject(size_t const vertexBufferSize, size_t const colorBufferSize);

	// inherited functions
    void setup(RenderEngineInterface&) final;
    void draw(RenderEngineInterface&, size_t const imageIndex) final;
    void cleanup(RenderEngineInterface&) final;

private:

	size_t const mVertexBufferSize;
	size_t const mColorBufferSize;
	uint32_t mInit = 0;

    MemoryMappedBuffer<VertexBufferElement> mVertexBuffer { vk::BufferUsageFlagBits::eVertexBuffer };
    MemoryMappedBuffer<ColorBufferElement2> mColorBuffer2 { vk::BufferUsageFlagBits::eStorageBuffer };
    MemoryMappedBuffer<UnformBuffer> mUniformBuffer { vk::BufferUsageFlagBits::eUniformBuffer };

    SimpleDescriptorSetLayout mDescriptorSetLayout;
    AdvancedDescriptorPool mDescriptorPool;
    AdvancedDescriptorSets mDescriptorSets;

    AdvancedGraphicsPipeline mPipeline;

	void recordCommands(RenderEngineInterface& engine);


	std::span<char const> getVertexShaderCode() const;
	std::span<char const> getGeometryShaderCode() const;
	std::span<char const> getFragmentShaderCode() const;

	vk::PrimitiveTopology getInputTopology() const;

    std::vector<vk::VertexInputAttributeDescription> getVertexAttributeDescriptions() const;
	std::vector<vk::VertexInputBindingDescription> getVertexBindingDescription() const;

	std::vector<vk::DescriptorSetLayoutBinding> getUniformBindingDescription() const;
	std::vector<vk::DescriptorPoolSize> getUniformDescriptorPoolSizes(uint32_t const swapChainSize) const;
};







