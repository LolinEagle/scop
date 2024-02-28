#include <MainClass.hpp>

using namespace std;

void	MainClass::loadGameObjects(void){
	shared_ptr<VeModel>	veModel = VeModel::createModelFromFile(_veDevice, "model/flat_vase.obj");
	auto				gameObject = VeGameObject::createGameObject();

	gameObject._model = veModel;
	gameObject._transform.translation = {.0f, .0f, 1.5f};
	gameObject._transform.scale = {.5f, .5f, .5f};
	_gameObjects.push_back(move(gameObject));
}

MainClass::MainClass(void){
	loadGameObjects();
}

MainClass::~MainClass(){
}

void	MainClass::run(void){
	vector<unique_ptr<VeBuffer>>	uboBuffers(VeSwapChain::MAX_FRAMES_IN_FLIGHT);
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

	SimpleRenderSystem	simpleRenderSystem{_veDevice, _veRenderer.getSwapchainRenderPass()};
	VeCamera			camera{};
	float				aspect;
	auto				viewerObject = VeGameObject::createGameObject();
	KeyboardController	cameraController{_veWindow.getGLFWwindow()};

	auto				currentTime = chrono::high_resolution_clock::now();
	auto				newTime = currentTime;
	float				frameTime;
	int					frameIndex;

	camera.setViewTarget(glm::vec3(-1.f, -2.f, -2.f), glm::vec3{.0f, .0f, 1.5f});
	while (!_veWindow.shouldClose()){
		glfwPollEvents();

		// Time
		newTime = chrono::high_resolution_clock::now();
		frameTime = chrono::duration<float, chrono::seconds::period>(newTime - currentTime).count();
		currentTime = newTime;

		cameraController.moveInPlaneXZ(_veWindow.getGLFWwindow(), frameTime, viewerObject);
		camera.setViewYXZ(viewerObject._transform.translation, viewerObject._transform.rotation);
		aspect = _veRenderer.getAspectRatio();
		camera.setPerspectiveProjection(glm::radians(50.), aspect, .1f, 10);
		if (auto commandBuffer = _veRenderer.beginFrame()){
			frameIndex = _veRenderer.getCurrentFrameIndex();
			FrameInfo	frameInfo{
				frameIndex,
				frameTime,
				commandBuffer,
				camera
			};

			// Update
			GlobalUbo	ubo{};
			ubo.projectionView = camera.getProjection() * camera.getView();
			uboBuffers[frameIndex]->writeToBuffer(&ubo);
			uboBuffers[frameIndex]->flush();

			// Render
			_veRenderer.beginSwapChainRenderPass(commandBuffer);
			simpleRenderSystem.renderObjects(frameInfo, _gameObjects);
			_veRenderer.endSwapChainRenderPass(commandBuffer);
			_veRenderer.endFrame();
		}
	}
	vkDeviceWaitIdle(_veDevice.device());
}
