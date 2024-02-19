#pragma once

#include <VeDevice.hpp>

using namespace std;

class VeModel{
	public:
		struct Vertex{
			glm::vec3	position;
			glm::vec3	color;

			static vector<VkVertexInputBindingDescription>		getBindingDescriptions(void);
			static vector<VkVertexInputAttributeDescription>	getAttributeDescriptions(void);
		};

		struct Builder{
			vector<Vertex>		vertices{};
			vector<uint32_t>	indices{};
		};
	private:
		VeDevice		&_veDevice;
		VkBuffer		_vertexBuffer;
		VkDeviceMemory	_vertexBufferMemory;
		uint32_t		_vertexCount;
		bool			_hasIndexBuffer = false;
		VkBuffer		_indexBuffer;
		VkDeviceMemory	_indexBufferMemory;
		uint32_t		_indexCount;

		void	createVertexBuffers(const vector<Vertex> &vertices);
		void	createIndexBuffers(const vector<uint32_t> &indices);
	public:
		VeModel(VeDevice &device, const VeModel::Builder &builder);
		~VeModel();

		void	bind(VkCommandBuffer commandBuffer);
		void	draw(VkCommandBuffer commandBuffer);
};
