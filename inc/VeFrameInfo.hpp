#pragma once

#include <VeCamera.hpp>

struct FrameInfo{
	int				frameIndex;
	float			frameTime;
	VkCommandBuffer	commandBuffer;
	VeCamera		&camera;
};
