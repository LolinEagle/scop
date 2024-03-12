#include <VeModel.hpp>

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
					vertices[indices[indices.size() - 1] - 0].position +
					vertices[indices[indices.size() - 2] - 0].position +
					vertices[indices[indices.size() - 3] - 0].position
				) / glm::vec3(3.f);
				for(; verticeAdded > 0; verticeAdded--)
					vertices[vertices.size() - verticeAdded].normal = tmp;
			}
		}
	}
	file.close();
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
