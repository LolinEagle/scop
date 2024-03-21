#include <VeModel.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace std;

template <typename T, typename... Rest>
void hashCombine(size_t &seed, const T &v, const Rest&... rest){
	seed ^= hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	(hashCombine(seed, rest), ...);
};

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
	ifstream			file(filepath);
	vector<glm::vec3>	positions, colors, normals;
	vector<glm::vec2>	uvs;
	string				line;

	while (getline(file, line)){
		istringstream	iss(line);
		string			token;
		iss >> token;
		if (token == "v"){// Vertex
			glm::vec3	position;
			iss >> position.x >> position.y >> position.z;
			positions.push_back(position);
		} else if (token == "vn"){// Normal
			glm::vec3	normal;
			iss >> normal.x >> normal.y >> normal.z;
			normals.push_back(normal);
		} else if (token == "vt"){// Texture
			glm::vec2	uv;
			iss >> uv.x >> uv.y;
			uvs.push_back(uv);
		} else if (token == "f"){// Face
			string		vertexIndices, indexStr;// Vertex indices, temporary index
			int			position, uv, normal;	// Position, texture, normal
			size_t		found;					// Found if face have uv and normal
			uint32_t	x[4];					// Keep indices for square face
			int			verticeAdded = 0;		// How many vertices have be added

			for (int i = 0; i < 4; i++){
				vertexIndices.clear();	// Clear vertex indices string
				iss >> vertexIndices;	// Get vertex indices

				// For corners with four face
				if (vertexIndices.empty())
					break ;
				if (i == 3){
					indices.push_back(x[0]);
					indices.push_back(x[2]);
				}

				istringstream	indicesStream(vertexIndices);	// Make it a input string stream
				found = vertexIndices.find('/');				// Find if face have uv and normal

				// Get from line vertex indices
				if (found != string::npos){
					getline(indicesStream, indexStr, '/');
					position = stoi(indexStr) - 1;
					getline(indicesStream, indexStr, '/');
					uv = stoi(indexStr) - 1;
					getline(indicesStream, indexStr, '/');
					normal = stoi(indexStr) - 1;
				} else {
					getline(indicesStream, indexStr);
					position = stoi(indexStr) - 1;
					uv = -1;
					normal = -1;
				}

				// Build the vertex
				Vertex	vertex{};
				vertex.position = positions[position];
				if (uv >= 0) vertex.uv = uvs[uv];
				if (normal >= 0) vertex.normal = normals[normal];
				vertex.color = glm::vec3(1.f, 1.f, 1.f);

				// Add it
				auto	it = find(vertices.begin(), vertices.end(), vertex);
				if (it == vertices.end()){
					vertices.push_back(vertex);
					verticeAdded++;
					x[i] = vertices.size() - 1;
					indices.push_back(x[i]);
				} else {
					x[i] = distance(vertices.begin(), it);
					indices.push_back(x[i]);
				}
			}
			// Build the vertex normal if not build already
			if (normal < 0){
				glm::vec3	tmp = glm::vec3(
					vertices[indices[indices.size() - 1]].position +
					vertices[indices[indices.size() - 2]].position +
					vertices[indices[indices.size() - 3]].position
				) / glm::vec3(3.f);
				for (; verticeAdded > 0; verticeAdded--)
					vertices[vertices.size() - verticeAdded].normal = tmp;
			}
		}
	}
	file.close();
}

void	VeModel::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height){
	VkCommandBuffer	commandBuffer = _veDevice.beginSingleTimeCommands();

	VkBufferImageCopy	region{};
	region.bufferOffset = 0;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;
	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;
	region.imageOffset = {0, 0, 0};
	region.imageExtent = {width, height, 1};

	vkCmdCopyBufferToImage(
		commandBuffer,
		buffer,
		image,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		1,
		&region
	);
	_veDevice.endSingleTimeCommands(commandBuffer);
}

void	VeModel::transitionImageLayout(
	VkImage image,VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout
){
	VkCommandBuffer	commandBuffer = _veDevice.beginSingleTimeCommands();

	VkImageMemoryBarrier	barrier{};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = oldLayout;
	barrier.newLayout = newLayout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = image;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;

	VkPipelineStageFlags	sourceStage;
	VkPipelineStageFlags	destinationStage;

	if (
		oldLayout == VK_IMAGE_LAYOUT_UNDEFINED &&
		newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
	){
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	} else if (
		oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
		newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
	){
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
		sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	} else {
		throw (invalid_argument("unsupported layout transition"));
	}
	vkCmdPipelineBarrier(
		commandBuffer,
		sourceStage, destinationStage,
		0,
		0, nullptr,
		0, nullptr,
		1, &barrier
	);
	_veDevice.endSingleTimeCommands(commandBuffer);
}

void	VeModel::createImage(
	uint32_t width, uint32_t height,
	VkFormat format,
	VkImageTiling tiling,
	VkImageUsageFlags usage,
	VkMemoryPropertyFlags properties,
	VkImage &image,
	VkDeviceMemory &imageMemory
){
	VkImageCreateInfo	imageInfo{};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = width;
	imageInfo.extent.height = height;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.format = format;
	imageInfo.tiling = tiling;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = usage;
	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateImage(_veDevice.device(), &imageInfo, nullptr, &image) != 0)
		throw (runtime_error("failed to create image"));

	VkMemoryRequirements	memRequirements;
	vkGetImageMemoryRequirements(_veDevice.device(), image, &memRequirements);

	VkMemoryAllocateInfo	allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = _veDevice.findMemoryType(memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(_veDevice.device(), &allocInfo, nullptr, &imageMemory) != 0)
		throw (runtime_error("failed to allocate image memory"));

	vkBindImageMemory(_veDevice.device(), image, imageMemory, 0);
}

void	VeModel::createTextureImages(void){
	int		texWidth, texHeight, texChannels;
	stbi_uc	*pixels = stbi_load(
		"model/texture/LolinEagle.png",
		&texWidth,
		&texHeight,
		&texChannels,
		STBI_rgb_alpha
	);

	if (!pixels)
		throw (runtime_error("image failed to load"));

	VkDeviceSize	imageSize = texWidth * texHeight * 4;
	VkBuffer		stagingBuffer;
	VkDeviceMemory	stagingBufferMemory;
	void			*data;

	_veDevice.createBuffer(
		imageSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		stagingBuffer,
		stagingBufferMemory
	);
	vkMapMemory(_veDevice.device(), stagingBufferMemory, 0, imageSize, 0, &data);
	memcpy(data, pixels, static_cast<size_t>(imageSize));
	vkUnmapMemory(_veDevice.device(), stagingBufferMemory);
	stbi_image_free(pixels);
	createImage(
		texWidth,
		texHeight,
		VK_FORMAT_R8G8B8A8_SRGB,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		_textureImage,
		_textureImageMemory
	);
	transitionImageLayout(
		_textureImage,
		VK_FORMAT_R8G8B8A8_SRGB,
		VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
	);
	copyBufferToImage(
		stagingBuffer,
		_textureImage,
		static_cast<uint32_t>(texWidth),
		static_cast<uint32_t>(texHeight)
	);
	transitionImageLayout(
		_textureImage,
		VK_FORMAT_R8G8B8A8_SRGB,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
	);
	vkDestroyBuffer(_veDevice.device(), stagingBuffer, nullptr);
	vkFreeMemory(_veDevice.device(), stagingBufferMemory, nullptr);
}

void	VeModel::createTextureImageView(void){
	VkImageViewCreateInfo	viewInfo{};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = _textureImage;
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
	viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = 1;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = 1;

	if (vkCreateImageView(_veDevice.device(), &viewInfo, nullptr, &_textureImageView) != 0)
		throw (runtime_error("failed to create texture image view"));
}

void	VeModel::createTextureSampler(void){
	VkSamplerCreateInfo	samplerInfo{};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.anisotropyEnable = VK_TRUE;
	samplerInfo.maxAnisotropy = 16.f;
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.mipLodBias = 0.f;
	samplerInfo.minLod = 0.f;
	samplerInfo.maxLod = 0.f;

	if (vkCreateSampler(_veDevice.device(), &samplerInfo, nullptr, &_textureSampler) != 0)
        throw (runtime_error("failed to create a sampler"));   
}

void	VeModel::createVertexBuffers(const vector<Vertex> &vertices){
	_vertexCount = static_cast<uint32_t>(vertices.size());
	if (_vertexCount < 3)
		throw (runtime_error("vertex count must be at least 3"));
	
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
	createTextureImages();
	createTextureImageView();
	createTextureSampler();
	createVertexBuffers(builder.vertices);
	createIndexBuffers(builder.indices);
}

VeModel::~VeModel(){
	vkDestroySampler(_veDevice.device(), _textureSampler, nullptr);
	vkDestroyImageView(_veDevice.device(), _textureImageView, nullptr);
	vkDestroyImage(_veDevice.device(), _textureImage, nullptr);
	vkFreeMemory(_veDevice.device(), _textureImageMemory, nullptr);
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

VkDescriptorImageInfo	VeModel::descriptorImageInfo(void){
	VkDescriptorImageInfo	imageInfo{};
	imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	imageInfo.imageView = _textureImageView;
	imageInfo.sampler = _textureSampler;
	return (imageInfo);
}
