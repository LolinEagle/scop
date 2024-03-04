#pragma once

#include <KeyboardController.hpp>
#include <SimpleRenderSystem.hpp>
#include <PointLightSystem.hpp>

#include <VeBuffer.hpp>
#include <VeCamera.hpp>
#include <VeDescriptors.hpp>
#include <VeDevice.hpp>
#include <VeGameObject.hpp>
#include <VeRenderer.hpp>
#include <VeWindow.hpp>

using namespace std;

struct GlobalUbo{
	glm::mat4	projection{1.f};
	glm::mat4	view{1.f};
	glm::vec4	ambientLightColor{1.f, 1.f, 1.f, .02f};
	glm::vec3	lightPosition{-1.f};
	alignas(16)
	glm::vec4	lightColor{1.f};
};

class MainClass{
	private:
		VeWindow	_veWindow{WIDTH, HEIGHT};
		VeDevice	_veDevice{_veWindow};
		VeRenderer	_veRenderer{_veWindow, _veDevice};

		unique_ptr<VeDescriptorPool>	_globalPool{};
		VeGameObject::Map				_gameObjects;

		void	loadGameObjects(
			const std::string &filepath, glm::vec3 translation, glm::vec3 scale
		);
	public:
		MainClass(void);
		~MainClass();

		void	run(void);
};
