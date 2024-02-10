#pragma once

#include <VeWindow.hpp>

using namespace std;

struct SwapChainSupportDetails{
	VkSurfaceCapabilitiesKHR	capabilities;
	vector<VkSurfaceFormatKHR>	formats;
	vector<VkPresentModeKHR>	presentModes;
};

struct QueueFamilyIndices{
	uint32_t	graphicsFamily;
	uint32_t	presentFamily;
	bool		graphicsFamilyHasValue = false;
	bool		presentFamilyHasValue = false;

	bool		isComplete(void);
};

class VeDevice{
	private:
		VkInstance					instance;
		VkDebugUtilsMessengerEXT	debugMessenger;
		VkPhysicalDevice			physicalDevice = VK_NULL_HANDLE;
		VeWindow					&window;
		VkCommandPool				commandPool;
		VkDevice					device_;
		VkSurfaceKHR				surface_;
		VkQueue 					graphicsQueue_;
		VkQueue						presentQueue_;
		const vector<const char*>	validationLayers = {"VK_LAYER_KHRONOS_validation"};
		const vector<const char*>	deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

		void	createInstance(void);
		void	setupDebugMessenger(void);
		void	createSurface(void);
		void	pickPhysicalDevice(void);
		void	createLogicalDevice(void);
		void	createCommandPool(void);

		// Helper functions
		bool					isDeviceSuitable(VkPhysicalDevice device);
		vector<const char*>		getRequiredExtensions(void);
		bool					checkValidationLayerSupport(void);
		QueueFamilyIndices		findQueueFamilies(VkPhysicalDevice device);
		void					populateDebugMessengerCreateInfo(
			VkDebugUtilsMessengerCreateInfoEXT &createInfo
		);
		void					hasGflwRequiredInstanceExtensions(void);
		bool					checkDeviceExtensionSupport(VkPhysicalDevice device);
		SwapChainSupportDetails	querySwapChainSupport(VkPhysicalDevice device);
	public:
		const bool 					enableValidationLayers = true;
		VkPhysicalDeviceProperties	properties;

		VeDevice(VeWindow &window);
		~VeDevice(void);

		VkCommandPool	getCommandPool(void);
		VkDevice		device(void);
		VkSurfaceKHR	surface(void);
		VkQueue			graphicsQueue(void);
		VkQueue			presentQueue(void);

		SwapChainSupportDetails	getSwapChainSupport(void);
		uint32_t				findMemoryType(
			uint32_t typeFilter,
			VkMemoryPropertyFlags properties
		);
		QueueFamilyIndices		findPhysicalQueueFamilies(void);
		VkFormat				findSupportedFormat(
			const vector<VkFormat> &candidates,
			VkImageTiling tiling,
			VkFormatFeatureFlags features
		);

		// Buffer Helper Functions
		void			createBuffer(
			VkDeviceSize size,
			VkBufferUsageFlags usage,
			VkMemoryPropertyFlags properties,
			VkBuffer &buffer,
			VkDeviceMemory &bufferMemory
		);
		VkCommandBuffer	beginSingleTimeCommands(void);
		void			endSingleTimeCommands(VkCommandBuffer commandBuffer);
		void			copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
		void			copyBufferToImage(
			VkBuffer buffer,
			VkImage image,
			uint32_t width,
			uint32_t height,
			uint32_t layerCount
		);
		void			createImageWithInfo(
			const VkImageCreateInfo &imageInfo,
			VkMemoryPropertyFlags properties,
			VkImage &image,
			VkDeviceMemory &imageMemory
		);
};
