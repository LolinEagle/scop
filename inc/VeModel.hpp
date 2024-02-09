#pragma once

#include <VeDevice.hpp>

using namespace std;

class VeModel{
	public:
		struct Vertex{
			glm::vec2	position;
			glm::vec3	color;

			static vector<VkVertexInputBindingDescription>		getBindingDescriptions(void);
			static vector<VkVertexInputAttributeDescription>	getAttributeDescriptions(void);
		};
	private:
		VeDevice		&_veDevice;
		VkBuffer		_vertexBuffer;
		VkDeviceMemory	_vertexBufferMemory;
		uint32_t		_vertexCount;

		void	createVertexBuffers(const vector<Vertex> &vertices);
	public:
		VeModel(VeDevice &device, const vector<Vertex> &vertices);
		~VeModel();

		void	bind(VkCommandBuffer commandBuffer);
		void	draw(VkCommandBuffer commandBuffer);
};
