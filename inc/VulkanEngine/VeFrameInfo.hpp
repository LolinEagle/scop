#pragma once

#include <VeCamera.hpp>
#include <VeGameObject.hpp>

struct PointLightEntity{
	glm::vec4	position{};
	glm::vec4	color{};
};

struct GlobalUbo{
	glm::mat4			projection{1.f};
	glm::mat4			view{1.f};
	glm::mat4			inverseView{1.f};
	glm::vec4			ambientLightColor{1.f, 1.f, 1.f, .02f};
	PointLightEntity	pointLights[MAX_LIGHTS];
	int					numLights;
};

struct FrameInfo{
	int					frameIndex;
	float				frameTime;
	VkCommandBuffer		commandBuffer;
	VeCamera			&camera;
	VkDescriptorSet		globalDescriptorSet;
	VeGameObject::Map	&gameObject;
};
