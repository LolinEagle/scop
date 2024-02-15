#include <MainClass.hpp>

using namespace std;

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
}

MainClass::~MainClass(){
}

void	MainClass::run(void){
	SimpleRenderSystem	simpleRenderSystem{_veDevice, _veRenderer.getSwapchainRenderPass()};

	while (!_veWindow.shouldClose()){
		glfwPollEvents();
		if (auto commandBuffer = _veRenderer.beginFrame()){
			_veRenderer.beginSwapChainRenderPass(commandBuffer);
			simpleRenderSystem.renderGameObjects(commandBuffer, _gameObjects);
			_veRenderer.endSwapChainRenderPass(commandBuffer);
			_veRenderer.endFrame();
		}
	}
	vkDeviceWaitIdle(_veDevice.device());
}
