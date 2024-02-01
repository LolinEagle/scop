#pragma once

#include <header.hpp>

class VePipeline
{
	private:
		static std::vector<char> readFile(const std::string &filepath);
		void createGraphicsPipeline(const std::string &vertFilepath, const std::string &fragFilepath);
	public:
		VePipeline(const std::string &vertFilepath, const std::string &fragFilepath);
		~VePipeline();
};
