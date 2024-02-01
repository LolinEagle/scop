#include <VeWindow.hpp>

void VeWindow::initWindow(void){
	if (!glfwInit())
		throw(std::string("Failed to initialize GLFW"));
	if (!glfwVulkanSupported())
		throw(std::string("Vulkan not supported"));
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	_window = glfwCreateWindow(_width, _height, TITLE, nullptr, nullptr);
	if (!_window)
		throw(std::string("Failed to create GLFW window"));
}

VeWindow::VeWindow(int width, int height) : _width(width), _height(height){
	initWindow();
}

VeWindow::~VeWindow(){
	glfwDestroyWindow(_window);
	glfwTerminate();
}

void VeWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface){
	// if (glfwCreateWindowSurface(instance, _window, nullptr, surface) != VK_SUCCESS)
	// 	throw(std::runtime_error("failed to create window surface"));
}

void VeWindow::run(void){
	while (!glfwWindowShouldClose(_window))
		glfwPollEvents();
}
