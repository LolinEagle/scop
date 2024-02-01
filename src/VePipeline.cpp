#include <VePipeline.hpp>

using namespace std;

vector<char> VePipeline::readFile(const string &filepath){
	ifstream file(filepath, ios::ate | ios::binary);

	if (!file.is_open())
		throw runtime_error("failed to open file: " + filepath);

	size_t fileSize = static_cast<size_t>(file.tellg());
	vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();
	return (buffer);
}

void VePipeline::createGraphicsPipeline(const string &vertFilepath, const string &fragFilepath){
	auto vertCode = readFile(vertFilepath);
	auto fragCode = readFile(fragFilepath);

	cout << "vertCode.size()=" << vertCode.size() << endl;
	cout << "fragCode.size()=" << fragCode.size() << endl;
}

VePipeline::VePipeline(const string &vertFilepath, const string &fragFilepath){
	createGraphicsPipeline(vertFilepath, fragFilepath);
}

VePipeline::~VePipeline(){
}
