#include <VeBuffer.hpp>
#include <cassert>

VkDeviceSize VeBuffer::getAlignment(VkDeviceSize instanceSize, VkDeviceSize minOffsetAlignment){
	if (minOffsetAlignment > 0){
		return (instanceSize + minOffsetAlignment - 1) & ~(minOffsetAlignment - 1);
	}
	return instanceSize;
}

VeBuffer::VeBuffer(
	VeDevice &device,
	VkDeviceSize instanceSize,
	uint32_t instanceCount,
	VkBufferUsageFlags usageFlags,
	VkMemoryPropertyFlags memoryPropertyFlags,
	VkDeviceSize minOffsetAlignment
):
	veDevice{device},
	instanceSize{instanceSize},
	instanceCount{instanceCount},
	usageFlags{usageFlags},
	memoryPropertyFlags{memoryPropertyFlags}
{
	alignmentSize = getAlignment(instanceSize, minOffsetAlignment);
	bufferSize = alignmentSize * instanceCount;
	device.createBuffer(bufferSize, usageFlags, memoryPropertyFlags, buffer, memory);
}

VeBuffer::~VeBuffer(){
	unmap();
	vkDestroyBuffer(veDevice.device(), buffer, nullptr);
	vkFreeMemory(veDevice.device(), memory, nullptr);
}

VkResult VeBuffer::map(VkDeviceSize size, VkDeviceSize offset){
	assert(buffer && memory && "Called map on buffer before create");
	return vkMapMemory(veDevice.device(), memory, offset, size, 0, &mapped);
}

void VeBuffer::unmap(){
	if (mapped){
		vkUnmapMemory(veDevice.device(), memory);
		mapped = nullptr;
	}
}

void VeBuffer::writeToBuffer(void *data, VkDeviceSize size, VkDeviceSize offset){
	assert(mapped && "Cannot copy to unmapped buffer");

	if (size == VK_WHOLE_SIZE){
		memcpy(mapped, data, bufferSize);
	} else {
		char *memOffset = (char *)mapped;
		memOffset += offset;
		memcpy(memOffset, data, size);
	}
}

VkResult VeBuffer::flush(VkDeviceSize size, VkDeviceSize offset){
	VkMappedMemoryRange mappedRange = {};
	mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
	mappedRange.memory = memory;
	mappedRange.offset = offset;
	mappedRange.size = size;
	return vkFlushMappedMemoryRanges(veDevice.device(), 1, &mappedRange);
}

VkResult VeBuffer::invalidate(VkDeviceSize size, VkDeviceSize offset){
	VkMappedMemoryRange mappedRange = {};
	mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
	mappedRange.memory = memory;
	mappedRange.offset = offset;
	mappedRange.size = size;
	return vkInvalidateMappedMemoryRanges(veDevice.device(), 1, &mappedRange);
}

VkDescriptorBufferInfo VeBuffer::descriptorInfo(VkDeviceSize size, VkDeviceSize offset){
	return VkDescriptorBufferInfo{
		buffer,
		offset,
		size,
	};
}

void VeBuffer::writeToIndex(void *data, int index){
	writeToBuffer(data, instanceSize, index * alignmentSize);
}

VkResult VeBuffer::flushIndex(int index){
	return flush(alignmentSize, index * alignmentSize);
}

VkDescriptorBufferInfo VeBuffer::descriptorInfoForIndex(int index){
	return descriptorInfo(alignmentSize, index * alignmentSize);
}

VkResult VeBuffer::invalidateIndex(int index){
	return invalidate(alignmentSize, index * alignmentSize);
}
