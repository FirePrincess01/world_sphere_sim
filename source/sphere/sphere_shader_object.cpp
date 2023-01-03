//
// @file:   sphere_shader_object.cpp
// @author: FirePrincess
// @date:   2022-01-02
// @brief:  Shader used to draw a sphere
//

#include "sphere_shader_object.h"


SphereShaderObject::SphereShaderObject(size_t const vertexBufferSize, size_t const colorBufferSize)
    : mVertexBufferSize(vertexBufferSize),
      mColorBufferSize(colorBufferSize)
{
	if(vertexBufferSize != mColorBufferSize * 6)
	{
		assert(false);
		throw std::exception("mIndexBufferSize != mColorBufferSize * 6");
	}
}


void SphereShaderObject::setup(RenderEngineInterface & engine)
{   
    // vertex buffer
    mVertexBuffer.create(engine, mVertexBufferSize);
    mColorBuffer2.create(engine, mColorBufferSize);
    
    // uniform buffer
    mUniformBuffer.create(engine, 1);
    mDescriptorSetLayout.createDescriptorSetLayout(engine, getUniformBindingDescription());
    mDescriptorPool.createDescriptorPool(engine, getUniformDescriptorPoolSizes(engine.getSwapChainSize()));

    mDescriptorSets.createDescriptorSets(engine,
        mDescriptorPool.getDescriptorPool(),
        mDescriptorSetLayout.getDescriptorSetLayout(),
        mUniformBuffer.getBuffers(),
        sizeof(UnformBuffer),
		mColorBuffer2.getBuffers(),
		sizeof(ColorBufferElement) * mColorBufferSize);

    // pipeline
    mPipeline.createGraphicsPipeline(engine, 
        getVertexShaderCode(),
        getGeometryShaderCode(),
        getFragmentShaderCode(),
        getVertexBindingDescription(),
        getVertexAttributeDescriptions(),
        mDescriptorSetLayout.getDescriptorSetLayout(),
        getInputTopology());

    // commands
    recordCommands(engine);
}

void SphereShaderObject::draw(RenderEngineInterface & engine, size_t const imageIndex)
{
	// init data
	if(mInit++ < engine.getSwapChainSize())
	{
		if(initVertexBuffer){
			mVertexBuffer.update(engine, imageIndex, initVertexBuffer);
		}

		if(initColorBuffer){
			mColorBuffer2.update(engine, imageIndex, initColorBuffer);
		}
	}

	// update data
	if(updateVertexBuffer){
		mVertexBuffer.update(engine, imageIndex, updateVertexBuffer);
	}

	if(updateColorBuffer){
		mColorBuffer2.update(engine, imageIndex, updateColorBuffer);
	}

	if(updateUniformBuffer){
		mUniformBuffer.update(engine, imageIndex, updateUniformBuffer);
	}
}

void SphereShaderObject::cleanup(RenderEngineInterface & engine)
{
    mPipeline.clear();

    mDescriptorSets.clear();
    mDescriptorPool.clear();
    mDescriptorSetLayout.clear();
    mUniformBuffer.clear();

    mColorBuffer2.clear();
    mVertexBuffer.clear();

	mInit = 0;
}

void SphereShaderObject::recordCommands(RenderEngineInterface& engine)
{
	assert(mPipeline.getPipelineLayout());
	assert(mPipeline.getPipeline());
	assert(!mVertexBuffer.getBuffers().empty());
	assert(!mColorBuffer2.getBuffers().empty());
	assert(!mUniformBuffer.getBuffers().empty());
	assert(!mDescriptorSets.getDescriptorSets().empty());

	auto & commandBuffers = engine.getCommandBuffers();

	for (size_t i = 0; i < commandBuffers.size(); i++)
	{
		commandBuffers[i]->bindPipeline(vk::PipelineBindPoint::eGraphics, mPipeline.getPipeline());

		commandBuffers[i]->bindVertexBuffers(0, mVertexBuffer.getBuffers()[i].get(), vk::DeviceSize(0));

		commandBuffers[i]->bindDescriptorSets(vk::PipelineBindPoint::eGraphics, mPipeline.getPipelineLayout(), 0, mDescriptorSets.getDescriptorSets()[i], nullptr);

		commandBuffers[i]->draw(static_cast<uint32_t>(mVertexBufferSize), 1, 0, 0);
	}
}


std::vector<vk::VertexInputAttributeDescription> SphereShaderObject::getVertexAttributeDescriptions() const
{
	std::vector<vk::VertexInputAttributeDescription> attributeDescriptions;
	attributeDescriptions.resize(2);
	attributeDescriptions[0].setBinding(0);
	attributeDescriptions[0].setLocation(0);
	attributeDescriptions[0].setFormat(vk::Format::eR32G32B32Sfloat);
	attributeDescriptions[0].setOffset(offsetof(VertexBufferElement, pos));

	attributeDescriptions[1].setBinding(0);
	attributeDescriptions[1].setLocation(1);
	attributeDescriptions[1].setFormat(vk::Format::eR32G32B32Sfloat);
	attributeDescriptions[1].setOffset(offsetof(VertexBufferElement, normal));

	return attributeDescriptions;
}

std::vector<vk::VertexInputBindingDescription> SphereShaderObject::getVertexBindingDescription() const
{
	std::vector<vk::VertexInputBindingDescription> bindingDescriptions;
    bindingDescriptions.resize(1);

	bindingDescriptions[0].setBinding(0);
	bindingDescriptions[0].setStride(sizeof(VertexBufferElement));
	bindingDescriptions[0].setInputRate(vk::VertexInputRate::eVertex);

	return bindingDescriptions;
}

std::vector<vk::DescriptorSetLayoutBinding> SphereShaderObject::getUniformBindingDescription() const
{
	// Uniform Buffer Layout
	std::vector<vk::DescriptorSetLayoutBinding>  uboLayoutBinding;
	uboLayoutBinding.resize(2);

	// Uniform Buffer Layout
	uboLayoutBinding[0].setBinding(0);
	uboLayoutBinding[0].setDescriptorType(vk::DescriptorType::eUniformBuffer);
	uboLayoutBinding[0].setDescriptorCount(1); // if it is an array of objects
	uboLayoutBinding[0].setStageFlags(vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment);
	uboLayoutBinding[0].setPImmutableSamplers(nullptr); // Optional

	// Storage Buffer Layout
	uboLayoutBinding[1].setBinding(1);
	uboLayoutBinding[1].setDescriptorType(vk::DescriptorType::eStorageBuffer);
	uboLayoutBinding[1].setDescriptorCount(1);	// if it is an array of objects
	uboLayoutBinding[1].setStageFlags(vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment);
	uboLayoutBinding[1].setPImmutableSamplers(nullptr); // Optional
	
	return uboLayoutBinding;
}

std::vector<vk::DescriptorPoolSize> SphereShaderObject::getUniformDescriptorPoolSizes(uint32_t const swapChainSize) const 
{
	std::vector<vk::DescriptorPoolSize> poolSize;
	poolSize.resize(2);

	// Uniform Buffer
	poolSize[0].setType(vk::DescriptorType::eUniformBuffer);
	poolSize[0].setDescriptorCount(swapChainSize);

	// Storage Buffer
	poolSize[1].setType(vk::DescriptorType::eStorageBuffer);
	poolSize[1].setDescriptorCount(swapChainSize);

	return poolSize;
}

#include "sphere_shader_vert.h"
std::span<char const> SphereShaderObject::getVertexShaderCode() const
{	
	return sphere_shader_vert;
}

std::span<char const> SphereShaderObject::getGeometryShaderCode() const
{	
	return std::span<char>();
}

#include "sphere_shader_frag.h"
std::span<char const> SphereShaderObject::getFragmentShaderCode() const
{
	return sphere_shader_frag;
}

vk::PrimitiveTopology SphereShaderObject::getInputTopology() const
{
	// return vk::PrimitiveTopology::ePointList;
	return vk::PrimitiveTopology::eTriangleList;
}
