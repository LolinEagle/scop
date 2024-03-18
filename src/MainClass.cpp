#include <MainClass.hpp>

using namespace std;

void	MainClass::loadGameObjects(const string &filepath, glm::vec3 translation, glm::vec3 scale){
	shared_ptr<VeModel>	veModel = VeModel::createModelFromFile(_veDevice, filepath);
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

	// Objects (filepath, translation, scale)
	const int	scene = 4;
	if (scene == 0) loadGameObjects("model/42.obj",			{0.f, 0.f, -1.f}, {1.f, -1.f, 1.f});
	if (scene == 1) loadGameObjects("model/cube.obj",		{0.f, 0.f,  0.f}, {1.f,  1.f, 1.f});
	if (scene == 2) loadGameObjects("model/teapot.obj",		{0.f, 0.f,  0.f}, {1.f, -1.f, 1.f});
	if (scene == 3) loadGameObjects("model/teapot2.obj",	{0.f, 0.f,  0.f}, {1.f, -1.f, 1.f});
	if (scene == 4){
		loadGameObjects("model/42.obj",			{-2.0f, -0.2f,  1.0f}, {1.0f, -1.0f, 1.0f});
		loadGameObjects("model/cube.obj",		{-2.0f,  0.0f, -2.0f}, {1.0f,  1.0f, 1.0f});
		loadGameObjects("model/teapot.obj",		{ 2.0f,  1.1f,  2.0f}, {1.0f, -1.0f, 1.0f});
		loadGameObjects("model/teapot2.obj",	{ 2.0f, -0.3f, -2.0f}, {1.0f, -1.0f, 1.0f});
		loadGameObjects("model/cube.obj",		{ 0.0f,  1.2f,  0.0f}, {8.0f,  0.1f, 8.0f});
	}

	// Lights
	vector<glm::vec3>	lightColors{
		{1.f, 0.f, 0.f},// Red
		{0.f, 1.f, 0.f},// Green
		{0.f, 0.f, 1.f},// Blue
		{1.f, 1.f, 0.f},// Yellow
		{0.f, 1.f, 1.f},// Cyan
		{1.f, 0.f, 1.f} // Purple
	};
	for (int i = 0; i < lightColors.size(); i++){
		auto	p = VeGameObject::makePointLight(8.f, .1f, glm::vec3(1.f));
		auto	rotateLight = glm::rotate(
			glm::mat4(2.2f),// Spreading
			(i * TWO_PI) / lightColors.size(),
			{0.f, -1.f, 0.f}// Axis
		);

		p._color = lightColors[i];
		p._transform.translation = glm::vec3(rotateLight * glm::vec4(-1.f, -1.f, -1.f, -1.f));
		_gameObjects.emplace(p.getId(), move(p));
	}
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
			_veDevice._properties.limits.minUniformBufferOffsetAlignment
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

	SimpleRender	simpleRenderSystem{
		_veDevice, _veRenderer.getSwapchainRenderPass(), globalSetLayout->getDescriptorSetLayout()
	};
	PointLight		pointLightSystem{
		_veDevice, _veRenderer.getSwapchainRenderPass(), globalSetLayout->getDescriptorSetLayout()
	};

	VeCamera	camera{};
	float		aspect;
	auto		viewerObject = VeGameObject::createGameObject();
	Controller	cameraController{_veWindow.getGLFWwindow()};

	// Time
	auto		currentTime = chrono::high_resolution_clock::now();
	auto		newTime = currentTime;
	float		frameTime;
	int			frameIndex;

	// Default value for camera
	viewerObject._transform.translation = {-8.f, -2.0f, .0f};
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
			ubo.projection = camera.getProjection();
			ubo.view = camera.getView();
			ubo.inverseView = camera.getInverseView();
			pointLightSystem.update(frameInfo, ubo);
			uboBuffers[frameIndex]->writeToBuffer(&ubo);
			uboBuffers[frameIndex]->flush();

			// Render
			_veRenderer.beginSwapChainRenderPass(commandBuffer);
			simpleRenderSystem.renderObjects(frameInfo);
			pointLightSystem.render(frameInfo);
			_veRenderer.endSwapChainRenderPass(commandBuffer);
			_veRenderer.endFrame();
		}
	}
	vkDeviceWaitIdle(_veDevice.device());
}
