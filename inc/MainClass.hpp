#pragma once

#include <KeyboardController.hpp>
#include <SimpleRenderSystem.hpp>

#include <VeBuffer.hpp>
#include <VeCamera.hpp>
#include <VeDescriptors.hpp>
#include <VeDevice.hpp>
#include <VeGameObject.hpp>
#include <VeRenderer.hpp>
#include <VeWindow.hpp>

using namespace std;

struct GlobalUbo{
	glm::mat4	projectionView{1.f};
	glm::vec3	lightDirection = glm::normalize(glm::vec3(1.f, -3.f, -1.f));
};

class MainClass{
	private:
		VeWindow	_veWindow{WIDTH, HEIGHT};
		VeDevice	_veDevice{_veWindow};
		VeRenderer	_veRenderer{_veWindow, _veDevice};

		unique_ptr<VeDescriptorPool>	_globalPool{};
		vector<VeGameObject>			_gameObjects;

		void	loadGameObjects(void);
	public:
		MainClass(void);
		~MainClass();

		void	run(void);
};
