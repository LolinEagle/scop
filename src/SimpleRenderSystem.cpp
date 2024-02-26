#include <SimpleRenderSystem.hpp>

void	SimpleRenderSystem::createPipelineLayout(void){
	VkPushConstantRange	pushConstantRange{};
	pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	pushConstantRange.offset = 0;
	pushConstantRange.size = sizeof(PushConstantData);

	VkPipelineLayoutCreateInfo	pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0;
	pipelineLayoutInfo.pSetLayouts = nullptr;
	pipelineLayoutInfo.pushConstantRangeCount = 1;
	pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
	if (vkCreatePipelineLayout(_veDevice.device(), &pipelineLayoutInfo, nullptr, &_pipelineLayout)
	!= VK_SUCCESS)
		throw (runtime_error("failed to create pipeline layout"));
}

void	SimpleRenderSystem::createPipeline(VkRenderPass renderPass){
	PipelineConfigInfo	pipelineConfig{};

	VePipeline::defaultPipelineConfigInfo(pipelineConfig);
	pipelineConfig.renderPass = renderPass;
	pipelineConfig.pipelineLayout = _pipelineLayout;
	_vePipeline = make_unique<VePipeline>(
		_veDevice,
		"shader/simpleShader.vert.spv",
		"shader/simpleShader.frag.spv",
		pipelineConfig
	);
}

SimpleRenderSystem::SimpleRenderSystem(VeDevice &veDevice, VkRenderPass renderPass)
: _veDevice(veDevice){
	createPipelineLayout();
	createPipeline(renderPass);
}

SimpleRenderSystem::~SimpleRenderSystem(){
	vkDestroyPipelineLayout(_veDevice.device(), _pipelineLayout, nullptr);
}

void	SimpleRenderSystem::renderGameObjects(
	VkCommandBuffer commandBuffer, vector<VeGameObject> &gameObjects, const VeCamera &camera
){
	auto	projectionView = camera.getProjection() * camera.getView();

	_vePipeline->bind(commandBuffer);
	for (auto &obj: gameObjects){
		PushConstantData	push{};
		auto				modelMatrix = obj._transform.mat4();
		push.transform = projectionView * obj._transform.mat4();
		push.normalMatrix = obj._transform.normalMatrix();
		vkCmdPushConstants(
			commandBuffer,
			_pipelineLayout,
			VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
			0,
			sizeof(PushConstantData),
			&push
		);
		obj._model->bind(commandBuffer);
		obj._model->draw(commandBuffer);
	}
}
