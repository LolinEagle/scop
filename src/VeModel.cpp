#include <VeModel.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#define GLM_ENABLE_EXPERIMENTAL
#include "../glm/gtx/hash.hpp"

using namespace std;

template <>
struct hash<VeModel::Vertex>{
	size_t	operator()(VeModel::Vertex const &vertex) const {
		size_t	seed = 0;
		hashCombine(seed, vertex.position, vertex.color, vertex.normal, vertex.uv);
		return (seed);
	}
};

vector<VkVertexInputBindingDescription>		VeModel::Vertex::getBindingDescriptions(void){
	vector<VkVertexInputBindingDescription>		bindingDescription(1);

	bindingDescription[0].binding = 0;
	bindingDescription[0].stride = sizeof(Vertex);
	bindingDescription[0].binding = VK_VERTEX_INPUT_RATE_VERTEX;
	return (bindingDescription);
}

vector<VkVertexInputAttributeDescription>	VeModel::Vertex::getAttributeDescriptions(void){
	vector<VkVertexInputAttributeDescription>	attributeDescription(2);

	attributeDescription[0].binding = 0;
	attributeDescription[0].location = 0;
	attributeDescription[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescription[0].offset = offsetof(Vertex, position);
	attributeDescription[1].binding = 0;
	attributeDescription[1].location = 1;
	attributeDescription[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescription[1].offset = offsetof(Vertex, color);
	return (attributeDescription);
}

bool	VeModel::Vertex::operator==(const Vertex &other) const {
	return (
		position == other.position &&
		color == other.color &&
		normal == other.normal &&
		uv == other.uv
	);
}

void	VeModel::Builder::loadModel(const string &filepath){
	tinyobj::attrib_t			attrib;
	vector<tinyobj::shape_t>	shapes;
	vector<tinyobj::material_t>	materials;
	string						warn, err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str()))
		throw (runtime_error(warn + err));
	vertices.clear();
	indices.clear();

	unordered_map<Vertex, uint32_t>	uniqueVertices;
	for (const auto &shape : shapes){
		for (const auto &index : shape.mesh.indices){
			Vertex	vertex{};

			// Vertex
			if (index.vertex_index >= 0){
				vertex.position = {
					attrib.vertices[3 * index.vertex_index + 0],
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2],
				};

				auto	colorIndex = 3 * index.vertex_index + 2;

				if (colorIndex < attrib.colors.size()){
					vertex.color = {
						attrib.colors[colorIndex - 2],
						attrib.colors[colorIndex - 1],
						attrib.colors[colorIndex - 0],
					};
				} else {
					vertex.color = {1.f, 1.f, 1.f};
				}
			}

			// Normal
			if (index.normal_index >= 0){
				vertex.normal = {
					attrib.normals[3 * index.normal_index + 0],
					attrib.normals[3 * index.normal_index + 1],
					attrib.normals[3 * index.normal_index + 2],
				};
			}

			// Texcoord
			if (index.texcoord_index >= 0){
				vertex.uv = {
					attrib.texcoords[2 * index.texcoord_index + 0],
					attrib.texcoords[2 * index.texcoord_index + 1],
				};
			}

			if (uniqueVertices.count(vertex) == 0){
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);
			}
			indices.push_back(uniqueVertices[vertex]);
		}
	}
}

void	VeModel::createVertexBuffers(const vector<Vertex> &vertices){
	_vertexCount = static_cast<uint32_t>(vertices.size());
	if (_vertexCount < 3)
		throw (runtime_error("Vertex count must be at least 3"));
	
	VkDeviceSize	bufferSize = sizeof(vertices[0]) * _vertexCount;
	VkBuffer		stagingBuffer;
	VkDeviceMemory	stagingBufferMemory;
	void			*data;

	_veDevice.createBuffer(
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		stagingBuffer,
		stagingBufferMemory
	);
	vkMapMemory(_veDevice.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
	vkUnmapMemory(_veDevice.device(), stagingBufferMemory);
	_veDevice.createBuffer(
		bufferSize,
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		_vertexBuffer,
		_vertexBufferMemory
	);
	_veDevice.copyBuffer(stagingBuffer, _vertexBuffer, bufferSize);
	vkDestroyBuffer(_veDevice.device(), stagingBuffer, nullptr);
	vkFreeMemory(_veDevice.device(), stagingBufferMemory, nullptr);
}

void	VeModel::createIndexBuffers(const vector<uint32_t> &indices){
	_indexCount = static_cast<uint32_t>(indices.size());
	_hasIndexBuffer = _indexCount > 0;
	if (!_hasIndexBuffer)
		return ;
	
	VkDeviceSize	bufferSize = sizeof(indices[0]) * _indexCount;
	VkBuffer		stagingBuffer;
	VkDeviceMemory	stagingBufferMemory;
	void			*data;

	_veDevice.createBuffer(
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		stagingBuffer,
		stagingBufferMemory
	);
	vkMapMemory(_veDevice.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, indices.data(), static_cast<size_t>(bufferSize));
	vkUnmapMemory(_veDevice.device(), stagingBufferMemory);
	_veDevice.createBuffer(
		bufferSize,
		VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		_indexBuffer,
		_indexBufferMemory
	);
	_veDevice.copyBuffer(stagingBuffer, _indexBuffer, bufferSize);
	vkDestroyBuffer(_veDevice.device(), stagingBuffer, nullptr);
	vkFreeMemory(_veDevice.device(), stagingBufferMemory, nullptr);
}

unique_ptr<VeModel>	VeModel::createModelFromFile(VeDevice &device, const string &filepath){
	Builder	builder{};
	builder.loadModel(filepath);
	return (make_unique<VeModel>(device, builder));
}

VeModel::VeModel(VeDevice &device, const VeModel::Builder &builder) : _veDevice(device){
	createVertexBuffers(builder.vertices);
	createIndexBuffers(builder.indices);
}

VeModel::~VeModel(){
	vkDestroyBuffer(_veDevice.device(), _vertexBuffer, nullptr);
	vkFreeMemory(_veDevice.device(), _vertexBufferMemory, nullptr);
	if (_hasIndexBuffer){
		vkDestroyBuffer(_veDevice.device(), _indexBuffer, nullptr);
		vkFreeMemory(_veDevice.device(), _indexBufferMemory, nullptr);
	}
}

void	VeModel::bind(VkCommandBuffer commandBuffer){
	VkBuffer		buffers[] = {_vertexBuffer};
	VkDeviceSize	offsets[] = {0};

	vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
	if (_hasIndexBuffer)
		vkCmdBindIndexBuffer(commandBuffer, _indexBuffer, 0, VK_INDEX_TYPE_UINT32);
}

void	VeModel::draw(VkCommandBuffer commandBuffer){
	if (_hasIndexBuffer)
		vkCmdDrawIndexed(commandBuffer, _indexCount, 1, 0, 0, 0);
	else
		vkCmdDraw(commandBuffer, _vertexCount, 1, 0, 0);
}
