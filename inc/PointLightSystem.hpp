#pragma once

#include <VeCamera.hpp>
#include <VeDevice.hpp>
#include <VeFrameInfo.hpp>
#include <VeGameObject.hpp>
#include <VePipeline.hpp>

using namespace std;

struct PointLightPushConstants{
	glm::vec4	position{};
	glm::vec4	color{};
	float		radius;
};

class PointLightSystem{
	private:
		VeDevice				&_veDevice;
		unique_ptr<VePipeline>	_vePipeline;
		VkPipelineLayout		_pipelineLayout;

		void	createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
		void	createPipeline(VkRenderPass renderPass);
	public:
		PointLightSystem(
			VeDevice &veDevice, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout
		);
		~PointLightSystem();

		void	update(FrameInfo &frameInfo, GlobalUbo &ubo);
		void	render(FrameInfo &frameInfo);
};
