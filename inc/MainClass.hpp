#pragma once

#include <VeDevice.hpp>
#include <VePipeline.hpp>
#include <VeSwapChain.hpp>
#include <VeWindow.hpp>

using namespace std;

class MainClass{
	private:
		VeWindow				veWindow{WIDTH, HEIGHT};
		VeDevice				veDevice{veWindow};
		VeSwapChain				veSwapChain{veDevice, veWindow.getExtent()};
		unique_ptr<VePipeline>	vePipeline;

		VkPipelineLayout		pipelineLayout;
		vector<VkCommandBuffer>	commandBuffers;

		void	createPipelineLayout(void);
		void	createPipeline(void);
		void	createCommandBuffers(void);
		void	drawFrame(void);
	public:
		MainClass(void);
		~MainClass();

		void	run(void);
};
