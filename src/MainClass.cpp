#include <MainClass.hpp>

using namespace std;

void	MainClass::createPipelineLayout(void){
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

void	MainClass::createPipeline(void){
	PipelineConfigInfo	pipelineConfig{};

	VePipeline::defaultPipelineConfigInfo(pipelineConfig);
	pipelineConfig.renderPass = _veSwapChain->getRenderPass();
	pipelineConfig.pipelineLayout = _pipelineLayout;
	_vePipeline = make_unique<VePipeline>(
		_veDevice,
		"shader/simpleShader.vert.spv",
		"shader/simpleShader.frag.spv",
		pipelineConfig
	);
}

void	MainClass::createCommandBuffers(void){
	_commandBuffers.resize(_veSwapChain->imageCount());

	VkCommandBufferAllocateInfo	allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = _veDevice.getCommandPool();
	allocInfo.commandBufferCount = static_cast<uint32_t>(_commandBuffers.size());

	if (vkAllocateCommandBuffers(_veDevice.device(), &allocInfo, _commandBuffers.data())
	!= VK_SUCCESS)
		throw (runtime_error("failed to allocate command buffers"));
}

void	MainClass::recreateSwapChain(void){
	auto	extent = _veWindow.getExtent();

	while (extent.width == 0 || extent.height == 0){
		extent = _veWindow.getExtent();
		glfwWaitEvents();
	}
	vkDeviceWaitIdle(_veDevice.device());
	if (_veSwapChain == nullptr){
		_veSwapChain = make_unique<VeSwapChain>(_veDevice, extent);
	} else {
		_veSwapChain = make_unique<VeSwapChain>(_veDevice, extent, move(_veSwapChain));
		if (_veSwapChain->imageCount() != _commandBuffers.size()){
			freeCommandBuffers();
			createCommandBuffers();
		}
	}
	createPipeline();
}

void	MainClass::recordCommandBuffers(int imageIndex){
	VkCommandBufferBeginInfo	beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	if (vkBeginCommandBuffer(_commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS)
		throw (runtime_error("failed to begin recording command buffer"));

	VkRenderPassBeginInfo	renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = _veSwapChain->getRenderPass();
	renderPassInfo.framebuffer = _veSwapChain->getFrameBuffer(imageIndex);

	renderPassInfo.renderArea.offset = {0, 0};
	renderPassInfo.renderArea.extent = _veSwapChain->getSwapChainExtent();

	array<VkClearValue, 2>	clearValues{};
	clearValues[0].color = {0.01, 0.01, 0.01, 1};
	clearValues[1].depthStencil = {1.0f, 0};
	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	vkCmdBeginRenderPass(_commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	VkViewport	viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(_veSwapChain->getSwapChainExtent().width);
	viewport.height = static_cast<float>(_veSwapChain->getSwapChainExtent().height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	VkRect2D scissor{{0, 0}, _veSwapChain->getSwapChainExtent()};
	vkCmdSetViewport(_commandBuffers[imageIndex], 0, 1, &viewport);
	vkCmdSetScissor(_commandBuffers[imageIndex], 0, 1, &scissor);
	renderGameObjects(_commandBuffers[imageIndex]);
	vkCmdEndRenderPass(_commandBuffers[imageIndex]);
	if (vkEndCommandBuffer(_commandBuffers[imageIndex]) != VK_SUCCESS)
		throw (runtime_error("failed to record command buffer"));
}

void	MainClass::freeCommandBuffers(void){
	vkFreeCommandBuffers(
		_veDevice.device(),
		_veDevice.getCommandPool(),
		static_cast<uint32_t>(_commandBuffers.size()),
		_commandBuffers.data()
	);
	_commandBuffers.clear();
}

void	MainClass::drawFrame(void){
	uint32_t	imageIndex;
	auto		result = _veSwapChain->acquireNextImage(&imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR){
		recreateSwapChain();
		return ;
	}
	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		throw (runtime_error("failed to acquire swap chain image"));
	recordCommandBuffers(imageIndex);
	result = _veSwapChain->submitCommandBuffers(&_commandBuffers[imageIndex], &imageIndex);
	if (result == VK_ERROR_OUT_OF_DATE_KHR ||
		result == VK_SUBOPTIMAL_KHR ||
		_veWindow.wasWindowResized()
	){
		_veWindow.resetWindowResizedFlag();
		recreateSwapChain();
		return ;
	}
	if (result != VK_SUCCESS)
		throw (runtime_error("failed to present swap chain image"));
}

void	MainClass::loadGameObjects(void){
	vector<VeModel::Vertex>	vertices{
		{{-0.6,  0.6}, {1, 0, 0}},	// Left
		{{ 0.6,  0.6}, {0, 1, 0}},	// Right
		{{ 0.0, -0.6}, {0, 0, 1}}	// Top
	};

	auto	veModel = make_shared<VeModel>(_veDevice, vertices);
	auto	triangle = VeGameObject::createGameObject();
	triangle._model = veModel;
	triangle._color = {.1f, .1f, .1f};
	triangle._transform2d.translation.x = .4f;
	triangle._transform2d.scale = {2.f, .5f};
	triangle._transform2d.rotation = .25f * glm::two_pi<float>();

	_gameObjects.push_back(move(triangle));
}

MainClass::MainClass(void){
	loadGameObjects();
	createPipelineLayout();
	recreateSwapChain();
	createCommandBuffers();
}

MainClass::~MainClass(){
	vkDestroyPipelineLayout(_veDevice.device(), _pipelineLayout, nullptr);
}

void	MainClass::run(void){
	while (!_veWindow.shouldClose()){
		glfwPollEvents();
		drawFrame();
	}
	vkDeviceWaitIdle(_veDevice.device());
}

void	MainClass::renderGameObjects(VkCommandBuffer commandBuffer){
	_vePipeline->bind(commandBuffer);
	for (auto &obj: _gameObjects){
		obj._transform2d.rotation = glm::mod(obj._transform2d.rotation + 0.001f, glm::two_pi<float>());

		PushConstantData	push{};
		push.offset = obj._transform2d.translation;
		push.color = obj._color;
		push.transform = obj._transform2d.mat2();
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
