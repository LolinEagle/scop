#include <MainClass.hpp>

using namespace std;

void	MainClass::loadGameObjects(
	const std::string &filepath, glm::vec3 translation, glm::vec3 scale
){
	shared_ptr<VeModel>	veModel = VeModel::createModelFromFile(
		_veDevice, "model/" + filepath + ".obj"
	);
	auto				gameObject = VeGameObject::createGameObject();

	gameObject._model = veModel;
	gameObject._transform.translation = translation;
	gameObject._transform.scale = scale;
	_gameObjects.emplace(gameObject.getId(), move(gameObject));
}

MainClass::MainClass(void){
	_globalPool = VeDescriptorPool::Builder(_veDevice)
		.setMaxSets(MAX_FRAMES_IN_FLIGHT)
		.addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, MAX_FRAMES_IN_FLIGHT)
		.build();
	loadGameObjects("42",			{.0f, .0f, -6.f}, {.5f, -.5f, .5f});
	loadGameObjects("colored_cube",	{.0f, .0f, -4.f}, {.5f, 0.5f, .5f});
	loadGameObjects("cube",			{.0f, .0f, -2.f}, {.5f, 0.5f, .5f});
	loadGameObjects("flat_vase",	{.0f, .5f, 0.0f}, {5.f, 5.0f, 5.f});
	loadGameObjects("smooth_vase",	{.0f, .5f, 2.0f}, {5.f, 5.0f, 5.f});
	loadGameObjects("teapot",		{.0f, .5f, 4.0f}, {.5f, -.5f, .5f});
	loadGameObjects("teapot2",		{.0f, .0f, 6.0f}, {.5f, -.5f, .5f});
	loadGameObjects("cube",			{.0f, .7f, 0.0f}, {4.f, 0.1f, 8.f});
}

MainClass::~MainClass(){
}

void	MainClass::run(void){
	vector<unique_ptr<VeBuffer>>	uboBuffers(MAX_FRAMES_IN_FLIGHT);
	for (int i = 0; i < uboBuffers.size(); i++){
		uboBuffers[i] = make_unique<VeBuffer>(
			_veDevice,
			sizeof(GlobalUbo),
			1,
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
			_veDevice.properties.limits.minUniformBufferOffsetAlignment
		);
		uboBuffers[i]->map();
	}

	auto					globalSetLayout = VeDescriptorSetLayout::Builder(_veDevice)
		.addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
		.build();
	vector<VkDescriptorSet>	globalDescriptorSets(MAX_FRAMES_IN_FLIGHT);
	for (int i = 0; i < globalDescriptorSets.size(); i++){
		auto	bufferInfo = uboBuffers[i]->descriptorInfo();
		VeDescriptorWriter(*globalSetLayout, *_globalPool)
			.writeBuffer(0, &bufferInfo)
			.build(globalDescriptorSets[i]);
	}

	SimpleRenderSystem	simpleRenderSystem{
		_veDevice, _veRenderer.getSwapchainRenderPass(), globalSetLayout->getDescriptorSetLayout()
	};
	VeCamera			camera{};
	float				aspect;
	auto				viewerObject = VeGameObject::createGameObject();
	KeyboardController	cameraController{_veWindow.getGLFWwindow()};

	auto				currentTime = chrono::high_resolution_clock::now();
	auto				newTime = currentTime;
	float				frameTime;
	int					frameIndex;

	// Default value for camera
	viewerObject._transform.translation = {-8.f, .0f, .0f};
	viewerObject._transform.rotation = {.0f, 1.5f, .0f};
	while (!_veWindow.shouldClose()){
		glfwPollEvents();

		// Time
		newTime = chrono::high_resolution_clock::now();
		frameTime = chrono::duration<float, chrono::seconds::period>(newTime - currentTime).count();
		currentTime = newTime;

		cameraController.moveInPlaneXZ(_veWindow.getGLFWwindow(), frameTime, viewerObject);
		camera.setViewYXZ(viewerObject._transform.translation, viewerObject._transform.rotation);
		aspect = _veRenderer.getAspectRatio();
		camera.setPerspectiveProjection(glm::radians(50.), aspect, .1f, 64.f);
		if (auto commandBuffer = _veRenderer.beginFrame()){
			frameIndex = _veRenderer.getCurrentFrameIndex();
			FrameInfo	frameInfo{
				frameIndex,
				frameTime,
				commandBuffer,
				camera,
				globalDescriptorSets[frameIndex],
				_gameObjects
			};

			// Update
			GlobalUbo	ubo{};
			ubo.projectionView = camera.getProjection() * camera.getView();
			uboBuffers[frameIndex]->writeToBuffer(&ubo);
			uboBuffers[frameIndex]->flush();

			// Render
			_veRenderer.beginSwapChainRenderPass(commandBuffer);
			simpleRenderSystem.renderObjects(frameInfo);
			_veRenderer.endSwapChainRenderPass(commandBuffer);
			_veRenderer.endFrame();
		}
	}
	vkDeviceWaitIdle(_veDevice.device());
}
