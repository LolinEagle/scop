#pragma once

#include <VeDevice.hpp>
#include <VeModel.hpp>

using namespace std;

struct PipelineConfigInfo{
	VkPipelineViewportStateCreateInfo		viewportInfo;
	VkPipelineInputAssemblyStateCreateInfo	inputAssemblyInfo;
	VkPipelineRasterizationStateCreateInfo	rasterizationInfo;
	VkPipelineMultisampleStateCreateInfo	multisampleInfo;
	VkPipelineColorBlendAttachmentState		colorBlendAttachment;
	VkPipelineColorBlendStateCreateInfo		colorBlendInfo;
	VkPipelineDepthStencilStateCreateInfo	depthStencilInfo;
	vector<VkDynamicState>					dynamicStateEnables;
	VkPipelineDynamicStateCreateInfo		dynamicStateInfo;
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

		static vector<char>	readFile(const string &filepath);
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

		void		bind(VkCommandBuffer commandBuffer);
		static void	defaultPipelineConfigInfo(PipelineConfigInfo &configInfo);
};
