#pragma once

#include <VeCamera.hpp>
#include <VeGameObject.hpp>

struct FrameInfo{
	int					frameIndex;
	float				frameTime;
	VkCommandBuffer		commandBuffer;
	VeCamera			&camera;
	VkDescriptorSet		globalDescriptorSet;
	VeGameObject::Map	&gameObject;
};
