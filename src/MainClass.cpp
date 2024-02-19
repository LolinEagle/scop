#include <MainClass.hpp>

using namespace std;

unique_ptr<VeModel>	createCubeModel(VeDevice& device, glm::vec3 offset){
	const glm::vec3	c[6] = {
		{.9f, .9f, .9f},// Left face (white)
		{.8f, .8f, .1f},// Right face (yellow)
		{.9f, .6f, .1f},// Top face (orange)
		{.8f, .1f, .1f},// Bottom face (red)
		{.1f, .1f, .8f},// Nose face (blue)
		{.1f, .8f, .1f}	// Tail face (green)
	};
	vector<VeModel::Vertex>	vertices{
		// Left face (white)
		{{-.5f, -.5f, -.5f}, c[0]},
		{{-.5f,  .5f,  .5f}, c[0]},
		{{-.5f, -.5f,  .5f}, c[0]},
		{{-.5f, -.5f, -.5f}, c[0]},
		{{-.5f,  .5f, -.5f}, c[0]},
		{{-.5f,  .5f,  .5f}, c[0]},

		// Right face (yellow)
		{{.5f, -.5f, -.5f}, c[1]},
		{{.5f,  .5f,  .5f}, c[1]},
		{{.5f, -.5f,  .5f}, c[1]},
		{{.5f, -.5f, -.5f}, c[1]},
		{{.5f,  .5f, -.5f}, c[1]},
		{{.5f,  .5f,  .5f}, c[1]},

		// Top face (orange, remember y axis points down)
		{{-.5f, -.5f, -.5f}, c[2]},
		{{ .5f, -.5f,  .5f}, c[2]},
		{{-.5f, -.5f,  .5f}, c[2]},
		{{-.5f, -.5f, -.5f}, c[2]},
		{{ .5f, -.5f, -.5f}, c[2]},
		{{ .5f, -.5f,  .5f}, c[2]},

		// Bottom face (red)
		{{-.5f, .5f, -.5f}, c[3]},
		{{ .5f, .5f,  .5f}, c[3]},
		{{-.5f, .5f,  .5f}, c[3]},
		{{-.5f, .5f, -.5f}, c[3]},
		{{ .5f, .5f, -.5f}, c[3]},
		{{ .5f, .5f,  .5f}, c[3]},

		// Nose face (blue)
		{{-.5f, -.5f, .5f}, c[4]},
		{{ .5f,  .5f, .5f}, c[4]},
		{{-.5f,  .5f, .5f}, c[4]},
		{{-.5f, -.5f, .5f}, c[4]},
		{{ .5f, -.5f, .5f}, c[4]},
		{{ .5f,  .5f, .5f}, c[4]},

		// Tail face (green)
		{{-.5f, -.5f, -.5f}, c[5]},
		{{ .5f,  .5f, -.5f}, c[5]},
		{{-.5f,  .5f, -.5f}, c[5]},
		{{-.5f, -.5f, -.5f}, c[5]},
		{{ .5f, -.5f, -.5f}, c[5]},
		{{ .5f,  .5f, -.5f}, c[5]},
	};
	for (auto& v : vertices){
		v.position += offset;
	}
	return (make_unique<VeModel>(device, vertices));
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

	// camera.setViewDirection(glm::vec3(0.f), glm::vec3(0.5f, 0.f, 1.f));
	camera.setViewTarget(glm::vec3(-1.f, -2.f, -2.f), glm::vec3{.0f, .0f, 1.5f});
	while (!_veWindow.shouldClose()){
		glfwPollEvents();
		aspect = _veRenderer.getAspectRatio();
		// camera.setOrthographicProjection(-aspect, aspect, -1, 1, -1, 1);
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
