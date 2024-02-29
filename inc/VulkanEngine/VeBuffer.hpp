#pragma once

#include <VeDevice.hpp>

class VeBuffer{
	private:
		VeDevice				&veDevice;
		void					*mapped = nullptr;
		VkBuffer				buffer = VK_NULL_HANDLE;
		VkDeviceMemory			memory = VK_NULL_HANDLE;

		VkDeviceSize			bufferSize;
		uint32_t				instanceCount;
		VkDeviceSize			instanceSize;
		VkDeviceSize			alignmentSize;
		VkBufferUsageFlags		usageFlags;
		VkMemoryPropertyFlags	memoryPropertyFlags;

		static VkDeviceSize	getAlignment(
			VkDeviceSize instanceSize, VkDeviceSize minOffsetAlignment
		);
	public:
		VeBuffer(
			VeDevice				&device,
			VkDeviceSize			instanceSize,
			uint32_t				instanceCount,
			VkBufferUsageFlags		usageFlags,
			VkMemoryPropertyFlags	memoryPropertyFlags,
			VkDeviceSize			minOffsetAlignment = 1
		);
		~VeBuffer();

		VkResult				map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
		void					unmap(void);

		void					writeToBuffer(
			void *data, VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0
		);
		VkResult				flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
		VkDescriptorBufferInfo	descriptorInfo(
			VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0
		);
		VkResult				invalidate(
			VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0
		);

		void					writeToIndex(void *data, int index);
		VkResult				flushIndex(int index);
		VkDescriptorBufferInfo	descriptorInfoForIndex(int index);
		VkResult				invalidateIndex(int index);

		VkBuffer				getBuffer(void) const {return(buffer);}
		void					*getMappedMemory(void) const {return(mapped);}
		uint32_t				getInstanceCount(void) const {return(instanceCount);}
		VkDeviceSize			getInstanceSize(void) const {return(instanceSize);}
		VkDeviceSize			getAlignmentSize(void) const {return(instanceSize);}
		VkBufferUsageFlags		getUsageFlags(void) const {return(usageFlags);}
		VkMemoryPropertyFlags	getMemoryPropertyFlags(void) const {return(memoryPropertyFlags);}
		VkDeviceSize			getBufferSize(void) const {return(bufferSize);}
};
