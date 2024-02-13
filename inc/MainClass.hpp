#pragma once

#include <VeDevice.hpp>
#include <VeGameObject.hpp>
#include <VeModel.hpp>
#include <VePipeline.hpp>
#include <VeSwapChain.hpp>
#include <VeWindow.hpp>

using namespace std;

struct PushConstantData{
	glm::mat2				transform{1.f};
	glm::vec2				offset;
	alignas(16) glm::vec3	color;
};

class MainClass{
	private:
		VeWindow				_veWindow{WIDTH, HEIGHT};
		VeDevice				_veDevice{_veWindow};
		unique_ptr<VeSwapChain>	_veSwapChain;
		unique_ptr<VePipeline>	_vePipeline;
		VkPipelineLayout		_pipelineLayout;
		vector<VkCommandBuffer>	_commandBuffers;
		vector<VeGameObject>	_gameObjects;

		void	createPipelineLayout(void);
		void	createPipeline(void);
		void	createCommandBuffers(void);
		void	freeCommandBuffers(void);
		void	drawFrame(void);
		void	loadGameObjects(void);
		void	recreateSwapChain(void);
		void	recordCommandBuffers(int imageIndex);
		void	renderGameObjects(VkCommandBuffer commandBuffer);
	public:
		MainClass(void);
		~MainClass();

		void	run(void);
};
