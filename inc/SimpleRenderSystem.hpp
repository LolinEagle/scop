#pragma once

#include <VeDevice.hpp>
#include <VeGameObject.hpp>
#include <VePipeline.hpp>

using namespace std;

struct PushConstantData{
	glm::mat2				transform{1.f};
	glm::vec2				offset;
	alignas(16) glm::vec3	color;
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

		void	renderGameObjects(
			VkCommandBuffer commandBuffer, vector<VeGameObject> &gameObjects
		);
};
