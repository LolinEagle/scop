#pragma once

#include <VeDevice.hpp>

using namespace std;

class VeSwapChain{
	private:
		VkFormat				swapChainImageFormat;
		VkFormat				swapChainDepthFormat;
		VkExtent2D				swapChainExtent;
		vector<VkFramebuffer>	swapChainFramebuffers;
		VkRenderPass			renderPass;
		vector<VkImage>			depthImages;
		vector<VkDeviceMemory>	depthImageMemorys;
		vector<VkImageView>		depthImageViews;
		vector<VkImage>			swapChainImages;
		vector<VkImageView>		swapChainImageViews;
		VeDevice				&device;
		VkExtent2D				windowExtent;
		VkSwapchainKHR			swapChain;
		shared_ptr<VeSwapChain>	oldSwapChain;
		vector<VkSemaphore>		imageAvailableSemaphores;
		vector<VkSemaphore>		renderFinishedSemaphores;
		vector<VkFence>			inFlightFences;
		vector<VkFence>			imagesInFlight;
		size_t					currentFrame = 0;

		void	init(void);
		void	createSwapChain(void);
		void	createImageViews(void);
		void	createDepthResources(void);
		void	createRenderPass(void);
		void	createFramebuffers(void);
		void	createSyncObjects(void);

		// Helper functions
		VkSurfaceFormatKHR	chooseSwapSurfaceFormat(const vector<VkSurfaceFormatKHR> &format);
		VkPresentModeKHR	chooseSwapPresentMode(const vector<VkPresentModeKHR> &present);
		VkExtent2D			chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);
	public:
		VeSwapChain(VeDevice &deviceRef, VkExtent2D extent);
		VeSwapChain(VeDevice &deviceRef, VkExtent2D extent, shared_ptr<VeSwapChain> previous);
		~VeSwapChain();

		VkFramebuffer	getFrameBuffer(int index);
		VkRenderPass	getRenderPass(void);
		VkImageView		getImageView(int index);
		size_t			imageCount(void);
		VkFormat		getSwapChainImageFormat(void);
		VkExtent2D		getSwapChainExtent(void);
		uint32_t		width(void);
		uint32_t		height(void);
		float			extentAspectRatio(void);
		VkFormat		findDepthFormat(void);
		VkResult		acquireNextImage(uint32_t *imageIndex);
		VkResult		submitCommandBuffers(const VkCommandBuffer *buffers, uint32_t *image);
		bool			compareSwapFormate(const VeSwapChain &swapChain) const;
};
