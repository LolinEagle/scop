#include <VeModel.hpp>

using namespace std;

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
