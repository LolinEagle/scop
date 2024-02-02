#pragma once

#include <header.hpp>
#include <VeDevice.hpp>

using namespace std;

struct PipelineConfigInfo {
	VkViewport								viewport;
	VkRect2D								scissor;
	VkPipelineViewportStateCreateInfo		viewportInfo;
	VkPipelineInputAssemblyStateCreateInfo	inputAssemblyInfo;
	VkPipelineRasterizationStateCreateInfo	rasterizationInfo;
	VkPipelineMultisampleStateCreateInfo	multisampleInfo;
	VkPipelineColorBlendAttachmentState		colorBlendAttachment;
	VkPipelineColorBlendStateCreateInfo		colorBlendInfo;
	VkPipelineDepthStencilStateCreateInfo	depthStencilInfo;
	VkPipelineLayout						pipelineLayout = nullptr;
	VkRenderPass							renderPass = nullptr;
	uint32_t								subpass = 0;
};

class VePipeline{
	private:
		VeDevice		&_veDevice;
		VkPipeline		_graphicsPipeline;
		VkShaderModule	_vertShaderModule;
		VkShaderModule	_fragShaderModule;

		static vector<char> readFile(const string &filepath);
		void	createGraphicsPipeline(
			const string &vertFilepath,
			const string &fragFilepath,
			const PipelineConfigInfo &configInfo
		);
		void	createShaderModule(const vector<char>& code, VkShaderModule *shaderModule);
	public:
		VePipeline(
			VeDevice &device,
			const string &vertFilepath,
			const string &fragFilepath,
			const PipelineConfigInfo &configInfo
		);
		~VePipeline();

		static PipelineConfigInfo	defaultPipelineConfigInfo(uint32_t width, uint32_t height);
};