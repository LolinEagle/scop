#include <VeModel.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#define GLM_ENABLE_EXPERIMENTAL
#include "../glm/gtx/hash.hpp"

namespace std{
	template <>
	struct hash<VeModel::Vertex>{
		size_t	operator()(VeModel::Vertex const &vertex) const {
			size_t	seed = 0;
			hashCombine(seed, vertex.position, vertex.color, vertex.normal, vertex.uv);
			return (seed);
		}
	};
}

using namespace std;

vector<VkVertexInputBindingDescription>		VeModel::Vertex::getBindingDescriptions(void){
	vector<VkVertexInputBindingDescription>		bindingDescription(1);

	bindingDescription[0].binding = 0;
	bindingDescription[0].stride = sizeof(Vertex);
	bindingDescription[0].binding = VK_VERTEX_INPUT_RATE_VERTEX;
	return (bindingDescription);
}

vector<VkVertexInputAttributeDescription>	VeModel::Vertex::getAttributeDescriptions(void){
	vector<VkVertexInputAttributeDescription>	attributeDescription{};

	// {binding, location, format, offset}
	attributeDescription.push_back({0 , 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, position)});
	attributeDescription.push_back({1 , 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color)});
	attributeDescription.push_back({2 , 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, normal)});
	attributeDescription.push_back({3 , 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, uv)});
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
				vertex.color = {
					attrib.colors[3 * index.vertex_index + 0],
					attrib.colors[3 * index.vertex_index + 1],
					attrib.colors[3 * index.vertex_index + 2],
				};
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
	uint32_t		vertexSize = sizeof(vertices[0]);
	VeBuffer		stagingBuffer{
		_veDevice,
		vertexSize,
		_vertexCount,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
	};

	stagingBuffer.map();
	stagingBuffer.writeToBuffer((void*)vertices.data());
	_vertexBuffer = make_unique<VeBuffer>(
		_veDevice,
		vertexSize,
		_vertexCount,
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
	);
	_veDevice.copyBuffer(stagingBuffer.getBuffer(), _vertexBuffer->getBuffer(), bufferSize);
}

void	VeModel::createIndexBuffers(const vector<uint32_t> &indices){
	_indexCount = static_cast<uint32_t>(indices.size());
	_hasIndexBuffer = _indexCount > 0;
	if (!_hasIndexBuffer)
		return ;
	
	VkDeviceSize	bufferSize = sizeof(indices[0]) * _indexCount;
	uint32_t		indexSize = sizeof(indices[0]);
	VeBuffer		stagingBuffer{
		_veDevice,
		indexSize,
		_indexCount,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
	};

	stagingBuffer.map();
	stagingBuffer.writeToBuffer((void*)indices.data());
	_indexBuffer = make_unique<VeBuffer>(
		_veDevice,
		indexSize,
		_indexCount,
		VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
	);
	_veDevice.copyBuffer(stagingBuffer.getBuffer(), _indexBuffer->getBuffer(), bufferSize);
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
}

void	VeModel::bind(VkCommandBuffer commandBuffer){
	VkBuffer		buffers[] = {_vertexBuffer->getBuffer()};
	VkDeviceSize	offsets[] = {0};

	vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
	if (_hasIndexBuffer)
		vkCmdBindIndexBuffer(commandBuffer, _indexBuffer->getBuffer(), 0, VK_INDEX_TYPE_UINT32);
}

void	VeModel::draw(VkCommandBuffer commandBuffer){
	if (_hasIndexBuffer)
		vkCmdDrawIndexed(commandBuffer, _indexCount, 1, 0, 0, 0);
	else
		vkCmdDraw(commandBuffer, _vertexCount, 1, 0, 0);
}
