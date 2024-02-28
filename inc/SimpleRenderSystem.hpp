#pragma once

#include <VeCamera.hpp>
#include <VeDevice.hpp>
#include <VeFrameInfo.hpp>
#include <VeGameObject.hpp>
#include <VePipeline.hpp>

using namespace std;

struct PushConstantData{
	glm::mat4	transform{1.f};
	glm::mat4	normalMatrix{1.f};
};

class SimpleRenderSystem{
	private:
		VeDevice				&_veDevice;
		unique_ptr<VePipeline>	_vePipeline;
		VkPipelineLayout		_pipelineLayout;

		void	createPipelineLayout(void);
		void	createPipeline(VkRenderPass renderPass);
	public:
		SimpleRenderSystem(VeDevice &veDevice, VkRenderPass renderPass);
		~SimpleRenderSystem();

		void	renderObjects(FrameInfo &frameInfo, vector<VeGameObject> &gameObjects);
};
