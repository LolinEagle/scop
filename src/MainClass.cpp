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
	SimpleRenderSystem	simpleRenderSystem{_veDevice, _veRenderer.getSwapchainRenderPass()};
	VeCamera			camera{};
	float				aspect;
	auto				viewerObject = VeGameObject::createGameObject();
	KeyboardController	cameraController{_veWindow.getGLFWwindow()};

	auto				currentTime = chrono::high_resolution_clock::now();
	auto				newTime = currentTime;
	float				frameTime;

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
			_veRenderer.beginSwapChainRenderPass(commandBuffer);
			simpleRenderSystem.renderGameObjects(commandBuffer, _gameObjects, camera);
			_veRenderer.endSwapChainRenderPass(commandBuffer);
			_veRenderer.endFrame();
		}
	}
	vkDeviceWaitIdle(_veDevice.device());
}
