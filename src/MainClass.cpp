#include <MainClass.hpp>

using namespace std;

void	MainClass::createPipelineLayout(void){
	VkPipelineLayoutCreateInfo	pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0;
	pipelineLayoutInfo.pSetLayouts = nullptr;
	pipelineLayoutInfo.pushConstantRangeCount = 0;
	pipelineLayoutInfo.pPushConstantRanges = nullptr;
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
	clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
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

	_vePipeline->bind(_commandBuffers[imageIndex]);
	_veModel->bind(_commandBuffers[imageIndex]);
	_veModel->draw(_commandBuffers[imageIndex]);

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

void	MainClass::loadModels(void){
	vector<VeModel::Vertex>	vertices{
		{{-1,  1}, {1, 0, 0}},	// Left
		{{ 1,  1}, {0, 1, 0}},	// Right
		{{ 0, -1}, {0, 0, 1}}	// Top
	};

	_veModel = make_unique<VeModel>(_veDevice, vertices);
}

MainClass::MainClass(void){
	loadModels();
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
