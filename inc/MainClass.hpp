#pragma once

#include <SimpleRenderSystem.hpp>
#include <VeCamera.hpp>
#include <VeDevice.hpp>
#include <VeGameObject.hpp>
#include <VeRenderer.hpp>
#include <VeWindow.hpp>

using namespace std;

class MainClass{
	private:
		VeWindow				_veWindow{WIDTH, HEIGHT};
		VeDevice				_veDevice{_veWindow};
		VeRenderer				_veRenderer{_veWindow, _veDevice};
		vector<VeGameObject>	_gameObjects;

		void	loadGameObjects(void);
	public:
		MainClass(void);
		~MainClass();

		void	run(void);
};
