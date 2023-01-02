//
// @file:   sphere_shader_object.cpp
// @author: FirePrincess
// @date:   2022-01-02
// @brief:  Shader used to draw a sphere
//

#include "sphere_shader_object.h"


SphereShaderObject::SphereShaderObject(size_t const vertexBufferSize, size_t const indexBufferSize)
    : mVertexBufferSize(vertexBufferSize),
      mIndexBufferSize(indexBufferSize)
{

}


void SphereShaderObject::setup(RenderEngineInterface & engine)
{   
    // vertex buffer
    mVertexBuffer.create(engine, mVertexBufferSize);
    mColorBuffer.create(engine, mVertexBufferSize);
    mIndexBuffer.create(engine, mIndexBufferSize);
    
    // uniform buffer
    mUniformBuffer.create(engine, 1);
    mDescriptorSetLayout.createDescriptorSetLayout(engine, getUniformBindingDescription());
    mDescriptorPool.createDescriptorPool(engine);
    mDescriptorSets.createDescriptorSets(engine,
        mDescriptorPool.getDescriptorPool(),
        mDescriptorSetLayout.getDescriptorSetLayout(),
        mUniformBuffer.getBuffers(),
        sizeof(UnformBuffer));

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
			mColorBuffer.update(engine, imageIndex, initColorBuffer);
		}

		if(initIndexBuffer){
			mIndexBuffer.update(engine, imageIndex, initIndexBuffer);
		}
	}

	// update data
	if(updateVertexBuffer){
		mVertexBuffer.update(engine, imageIndex, updateVertexBuffer);
	}

	if(updateColorBuffer){
		mColorBuffer.update(engine, imageIndex, updateColorBuffer);
	}

	if(updateIndexBuffer){
		mIndexBuffer.update(engine, imageIndex, updateIndexBuffer);
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

    mIndexBuffer.clear();
    mColorBuffer.clear();
    mVertexBuffer.clear();

	mInit = 0;
}

void SphereShaderObject::recordCommands(RenderEngineInterface& engine)
{
	assert(mPipeline.getPipelineLayout());
	assert(mPipeline.getPipeline());
	assert(!mVertexBuffer.getBuffers().empty());
	assert(!mColorBuffer.getBuffers().empty());
	assert(!mIndexBuffer.getBuffers().empty());
	assert(!mUniformBuffer.getBuffers().empty());
	assert(!mDescriptorSets.getDescriptorSets().empty());

	auto & commandBuffers = engine.getCommandBuffers();

	for (size_t i = 0; i < commandBuffers.size(); i++)
	{
		commandBuffers[i]->bindPipeline(vk::PipelineBindPoint::eGraphics, mPipeline.getPipeline());

		commandBuffers[i]->bindVertexBuffers(0, mVertexBuffer.getBuffers()[i].get(), vk::DeviceSize(0));
		commandBuffers[i]->bindVertexBuffers(1, mColorBuffer.getBuffers()[i].get(), vk::DeviceSize(0));
        commandBuffers[i]->bindIndexBuffer(mIndexBuffer.getBuffers()[i].get(), 0, vk::IndexType::eUint32);

		commandBuffers[i]->bindDescriptorSets(vk::PipelineBindPoint::eGraphics, mPipeline.getPipelineLayout(), 0, mDescriptorSets.getDescriptorSets()[i], nullptr);

		commandBuffers[i]->draw(static_cast<uint32_t>(mVertexBufferSize), 1, 0, 0);
		commandBuffers[i]->drawIndexed(mIndexBufferSize, 1, 0, 0, 0);
	}
}


std::vector<vk::VertexInputAttributeDescription> SphereShaderObject::getVertexAttributeDescriptions() const
{
	std::vector<vk::VertexInputAttributeDescription> attributeDescriptions;
	attributeDescriptions.resize(3);
	attributeDescriptions[0].setBinding(0);
	attributeDescriptions[0].setLocation(0);
	attributeDescriptions[0].setFormat(vk::Format::eR32G32B32Sfloat);
	attributeDescriptions[0].setOffset(offsetof(VertexBufferElement, pos));

	attributeDescriptions[1].setBinding(0);
	attributeDescriptions[1].setLocation(1);
	attributeDescriptions[1].setFormat(vk::Format::eR32G32B32Sfloat);
	attributeDescriptions[1].setOffset(offsetof(VertexBufferElement, normal));

	attributeDescriptions[2].setBinding(1);
	attributeDescriptions[2].setLocation(2);
	attributeDescriptions[2].setFormat(vk::Format::eR32G32B32Sfloat);
	attributeDescriptions[2].setOffset(offsetof(ColorBufferElement, color));

	return attributeDescriptions;
}

std::vector<vk::VertexInputBindingDescription> SphereShaderObject::getVertexBindingDescription() const
{
	std::vector<vk::VertexInputBindingDescription> bindingDescriptions;
    bindingDescriptions.resize(2);
	bindingDescriptions[0].setBinding(0);
	bindingDescriptions[0].setStride(sizeof(VertexBufferElement));
	bindingDescriptions[0].setInputRate(vk::VertexInputRate::eVertex);

	bindingDescriptions[1].setBinding(1);
	bindingDescriptions[1].setStride(sizeof(ColorBufferElement));
	bindingDescriptions[1].setInputRate(vk::VertexInputRate::eVertex);

	return bindingDescriptions;
}

std::vector<vk::DescriptorSetLayoutBinding> SphereShaderObject::getUniformBindingDescription() const
{
	// Uniform Buffer Layout
	std::vector<vk::DescriptorSetLayoutBinding>  uboLayoutBinding;
	uboLayoutBinding.resize(1);
	uboLayoutBinding[0].setBinding(0);
	uboLayoutBinding[0].setDescriptorType(vk::DescriptorType::eUniformBuffer);
	uboLayoutBinding[0].setDescriptorCount(1); // if it is an array of objects
	uboLayoutBinding[0].setStageFlags(vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment);
	uboLayoutBinding[0].setPImmutableSamplers(nullptr); // Optional

	return uboLayoutBinding;
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
	return vk::PrimitiveTopology::eTriangleList;
}
