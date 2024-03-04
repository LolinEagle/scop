#include <PointLightSystem.hpp>

void	PointLightSystem::createPipelineLayout(VkDescriptorSetLayout globalSetLayout){
	// VkPushConstantRange	pushConstantRange{};
	// pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	// pushConstantRange.offset = 0;
	// pushConstantRange.size = sizeof(PushConstantData);

	vector<VkDescriptorSetLayout>	descriptorSetLayouts{globalSetLayout};

	VkPipelineLayoutCreateInfo	pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
	pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
	pipelineLayoutInfo.pushConstantRangeCount = 0;
	pipelineLayoutInfo.pPushConstantRanges = nullptr;
	if (vkCreatePipelineLayout(_veDevice.device(), &pipelineLayoutInfo, nullptr, &_pipelineLayout)
	!= VK_SUCCESS)
		throw (runtime_error("failed to create pipeline layout"));
}

void	PointLightSystem::createPipeline(VkRenderPass renderPass){
	PipelineConfigInfo	pipelineConfig{};

	VePipeline::defaultPipelineConfigInfo(pipelineConfig);
	pipelineConfig.attributeDescription.clear();
	pipelineConfig.bindingDescription.clear();
	pipelineConfig.renderPass = renderPass;
	pipelineConfig.pipelineLayout = _pipelineLayout;
	_vePipeline = make_unique<VePipeline>(
		_veDevice,
		"shader/pointLight.vert.spv",
		"shader/pointLight.frag.spv",
		pipelineConfig
	);
}

PointLightSystem::PointLightSystem(
	VeDevice &veDevice, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout
) : _veDevice(veDevice){
	createPipelineLayout(globalSetLayout);
	createPipeline(renderPass);
}

PointLightSystem::~PointLightSystem(){
	vkDestroyPipelineLayout(_veDevice.device(), _pipelineLayout, nullptr);
}

void	PointLightSystem::render(FrameInfo &frameInfo){
	_vePipeline->bind(frameInfo.commandBuffer);
	vkCmdBindDescriptorSets(
		frameInfo.commandBuffer,
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		_pipelineLayout,
		0,
		1,
		&frameInfo.globalDescriptorSet,
		0,
		nullptr
	);
	vkCmdDraw(frameInfo.commandBuffer, 6, 1, 0, 0);
}
