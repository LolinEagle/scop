#pragma once

#include <VeDevice.hpp>
#include <VeModel.hpp>
#include <VePipeline.hpp>
#include <VeSwapChain.hpp>
#include <VeWindow.hpp>

using namespace std;

class MainClass{
	private:
		VeWindow				_veWindow{WIDTH, HEIGHT};
		VeDevice				_veDevice{_veWindow};
		unique_ptr<VeSwapChain>	_veSwapChain;
		unique_ptr<VePipeline>	_vePipeline;
		VkPipelineLayout		_pipelineLayout;
		vector<VkCommandBuffer>	_commandBuffers;
		unique_ptr<VeModel>		_veModel;

		void	createPipelineLayout(void);
		void	createPipeline(void);
		void	createCommandBuffers(void);
		void	freeCommandBuffers(void);
		void	drawFrame(void);
		void	loadModels(void);
		void	recreateSwapChain(void);
		void	recordCommandBuffers(int imageIndex);
	public:
		MainClass(void);
		~MainClass();

		void	run(void);
};
