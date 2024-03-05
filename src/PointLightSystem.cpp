#include <PointLightSystem.hpp>

void	PointLightSystem::createPipelineLayout(VkDescriptorSetLayout globalSetLayout){
	VkPushConstantRange	pushConstantRange{};
	pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	pushConstantRange.offset = 0;
	pushConstantRange.size = sizeof(PointLightPushConstants);

	vector<VkDescriptorSetLayout>	descriptorSetLayouts{globalSetLayout};

	VkPipelineLayoutCreateInfo	pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
	pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
	pipelineLayoutInfo.pushConstantRangeCount = 1;
	pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
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

void	PointLightSystem::update(FrameInfo &frameInfo, GlobalUbo &ubo){
	auto	rotateLight = glm::rotate(glm::mat4(1.f), frameInfo.frameTime, {0.f, -1.f, 0.f});
	int		lightIndex = 0;

	for (auto &kv: frameInfo.gameObject){
		auto	&obj = kv.second;

		if (obj._pointLight == nullptr)
			continue ;
		// Time
		obj._transform.translation = glm::vec3(
			rotateLight * glm::vec4(obj._transform.translation, 1.f)
		);
		ubo.pointLights[lightIndex].position = glm::vec4(obj._transform.translation, 1.f);
		ubo.pointLights[lightIndex].color = glm::vec4(obj._color, obj._pointLight->lightIntensity);
		lightIndex++;
	}
	ubo.numLights = lightIndex;
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
	for (auto &kv: frameInfo.gameObject){
		auto	&obj = kv.second;

		if (obj._pointLight == nullptr)
			continue ;
		PointLightPushConstants	push{};
		push.position = glm::vec4(obj._transform.translation, 1.f);
		push.color = glm::vec4(obj._color, obj._pointLight->lightIntensity);
		push.radius = obj._transform.scale.x;
		vkCmdPushConstants(
			frameInfo.commandBuffer,
			_pipelineLayout,
			VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
			0,
			sizeof(PointLightPushConstants),
			&push
		);
		vkCmdDraw(frameInfo.commandBuffer, 6, 1, 0, 0);
	}
}
