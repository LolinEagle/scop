#include <MainClass.hpp>

using namespace std;

unique_ptr<VeModel>	createCubeModel(VeDevice& device, glm::vec3 offset){
	VeModel::Builder	modelBuilder{};
	const glm::vec3		c[6] = {
		{.9f, .9f, .9f},// Left face (white)
		{.8f, .8f, .1f},// Right face (yellow)
		{.9f, .6f, .1f},// Top face (orange)
		{.8f, .1f, .1f},// Bottom face (red)
		{.1f, .1f, .8f},// Nose face (blue)
		{.1f, .8f, .1f}	// Tail face (green)
	};

	modelBuilder.vertices = {
		// Left face (white)
		{{-.5f, -.5f, -.5f}, c[0]},
		{{-.5f,  .5f,  .5f}, c[0]},
		{{-.5f, -.5f,  .5f}, c[0]},
		{{-.5f,  .5f, -.5f}, c[0]},

		// Right face (yellow)
		{{.5f, -.5f, -.5f}, c[1]},
		{{.5f,  .5f,  .5f}, c[1]},
		{{.5f, -.5f,  .5f}, c[1]},
		{{.5f,  .5f, -.5f}, c[1]},

		// Top face (orange, remember y axis points down)
		{{-.5f, -.5f, -.5f}, c[2]},
		{{ .5f, -.5f,  .5f}, c[2]},
		{{-.5f, -.5f,  .5f}, c[2]},
		{{ .5f, -.5f, -.5f}, c[2]},

		// Bottom face (red)
		{{-.5f, .5f, -.5f}, c[3]},
		{{ .5f, .5f,  .5f}, c[3]},
		{{-.5f, .5f,  .5f}, c[3]},
		{{ .5f, .5f, -.5f}, c[3]},

		// Nose face (blue)
		{{-.5f, -.5f, .5f}, c[4]},
		{{ .5f,  .5f, .5f}, c[4]},
		{{-.5f,  .5f, .5f}, c[4]},
		{{ .5f, -.5f, .5f}, c[4]},

		// Tail face (green)
		{{-.5f, -.5f, -.5f}, c[5]},
		{{ .5f,  .5f, -.5f}, c[5]},
		{{-.5f,  .5f, -.5f}, c[5]},
		{{ .5f, -.5f, -.5f}, c[5]},
	};
	for (auto& v : modelBuilder.vertices){
		v.position += offset;
	}
	modelBuilder.indices = {0, 1, 2, 0, 3, 1, 4, 5, 6, 4, 7, 5, 8, 9, 10, 8, 11, 9, 12, 13, 14, 12, 15, 13, 16, 17, 18, 16, 19, 17, 20, 21, 22, 20, 23, 21};
	return (make_unique<VeModel>(device, modelBuilder));
}

void	MainClass::loadGameObjects(void){
	shared_ptr<VeModel>	veModel = createCubeModel(_veDevice, {.0f, .0f, .0f});
	auto				cube = VeGameObject::createGameObject();

	cube._model = veModel;
	cube._transform.translation = {.0f, .0f, 1.5f};
	cube._transform.scale = {.5f, .5f, .5f};
	_gameObjects.push_back(move(cube));
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
