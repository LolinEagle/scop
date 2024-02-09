#include <MainClass.hpp>

using namespace std;

void	MainClass::createPipelineLayout(void){
	VkPipelineLayoutCreateInfo	pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0;
	pipelineLayoutInfo.pSetLayouts = nullptr;
	pipelineLayoutInfo.pushConstantRangeCount = 0;
	pipelineLayoutInfo.pPushConstantRanges = nullptr;
	if (vkCreatePipelineLayout(veDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
		throw (runtime_error("failed to create pipeline layout"));
}

void	MainClass::createPipeline(void){
	PipelineConfigInfo	pipelineConfig{};

	VePipeline::defaultPipelineConfigInfo(pipelineConfig);
	pipelineConfig.renderPass = veSwapChain->getRenderPass();
	pipelineConfig.pipelineLayout = pipelineLayout;
	vePipeline = make_unique<VePipeline>(
		veDevice,
		"shader/simpleShader.vert.spv",
		"shader/simpleShader.frag.spv",
		pipelineConfig
	);
}

void	MainClass::createCommandBuffers(void){
	commandBuffers.resize(veSwapChain->imageCount());

	VkCommandBufferAllocateInfo	allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = veDevice.getCommandPool();
	allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

	if (vkAllocateCommandBuffers(veDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
		throw (runtime_error("failed to allocate command buffers"));
}

void	MainClass::recreateSwapChain(void){
	auto	extent = veWindow.getExtent();

	while (extent.width == 0 || extent.height == 0){
		extent = veWindow.getExtent();
		glfwWaitEvents();
	}
	vkDeviceWaitIdle(veDevice.device());
	if (veSwapChain == nullptr){
		veSwapChain = make_unique<VeSwapChain>(veDevice, extent);
	} else {
		veSwapChain = make_unique<VeSwapChain>(veDevice, extent, move(veSwapChain));
		if (veSwapChain->imageCount() != commandBuffers.size()){
			freeCommandBuffers();
			createCommandBuffers();
		}
	}
	createPipeline();
}

void	MainClass::recordCommandBuffers(int imageIndex){
	VkCommandBufferBeginInfo	beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS)
		throw (runtime_error("failed to begin recording command buffer"));

	VkRenderPassBeginInfo	renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = veSwapChain->getRenderPass();
	renderPassInfo.framebuffer = veSwapChain->getFrameBuffer(imageIndex);

	renderPassInfo.renderArea.offset = {0, 0};
	renderPassInfo.renderArea.extent = veSwapChain->getSwapChainExtent();

	array<VkClearValue, 2>	clearValues{};
	clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
	clearValues[1].depthStencil = {1.0f, 0};
	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	VkViewport	viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(veSwapChain->getSwapChainExtent().width);
	viewport.height = static_cast<float>(veSwapChain->getSwapChainExtent().height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	VkRect2D scissor{{0, 0}, veSwapChain->getSwapChainExtent()};
	vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
	vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

	vePipeline->bind(commandBuffers[imageIndex]);
	veModel->bind(commandBuffers[imageIndex]);
	veModel->draw(commandBuffers[imageIndex]);

	vkCmdEndRenderPass(commandBuffers[imageIndex]);
	if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS)
		throw (runtime_error("failed to record command buffer"));
}

void	MainClass::freeCommandBuffers(void){
	vkFreeCommandBuffers(
		veDevice.device(),
		veDevice.getCommandPool(),
		static_cast<uint32_t>(commandBuffers.size()),
		commandBuffers.data()
	);
	commandBuffers.clear();
}

void	MainClass::drawFrame(void){
	uint32_t	imageIndex;
	auto		result = veSwapChain->acquireNextImage(&imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR){
		recreateSwapChain();
		return ;
	}
	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		throw (runtime_error("failed to acquire swap chain image"));
	recordCommandBuffers(imageIndex);
	result = veSwapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || veWindow.wasWindowResized()){
		veWindow.resetWindowResizedFlag();
		recreateSwapChain();
		return ;
	}
	if (result != VK_SUCCESS)
		throw (runtime_error("failed to present swap chain image"));
}

void 	sierpinski(
	vector<VeModel::Vertex> &vertices,
	int						depth,
	glm::vec2				top,
	glm::vec2				left,
	glm::vec2				right
){
	if (depth > 16)
		throw (runtime_error("Too many depth"));
	if (depth <= 0){
		vertices.push_back({top});
		vertices.push_back({right});
		vertices.push_back({left});
	} else {
		auto	topLeft = (top + left) * 0.5f;
		auto	leftRight = (left + right) * 0.5f;
		auto	rightTop = (right + top) * 0.5f;
		sierpinski(vertices, depth - 1, top, topLeft, rightTop);
		sierpinski(vertices, depth - 1, topLeft, left, leftRight);
		sierpinski(vertices, depth - 1, rightTop, leftRight, right);
	}
}

void	MainClass::loadModels(void){
	vector<VeModel::Vertex>	vertices{
		{{-1,  1}, {1, 0, 0}},	// Left
		{{ 1,  1}, {0, 1, 0}},	// Right
		{{ 0, -1}, {0, 0, 1}}	// Top
	};

	veModel = make_unique<VeModel>(veDevice, vertices);
}

MainClass::MainClass(void){
	loadModels();
	createPipelineLayout();
	recreateSwapChain();
	createCommandBuffers();
}

MainClass::~MainClass(){
	vkDestroyPipelineLayout(veDevice.device(), pipelineLayout, nullptr);
}

void	MainClass::run(void){
	while (!veWindow.shouldClose()){
		glfwPollEvents();
		drawFrame();
	}
	vkDeviceWaitIdle(veDevice.device());
}
