#include <VeWindow.hpp>

using namespace std;

void	VeWindow::initWindow(void){
	if (!glfwInit())
		throw (string("Failed to initialize GLFW"));
	if (!glfwVulkanSupported())
		throw (string("Vulkan not supported"));
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	_window = glfwCreateWindow(_width, _height, TITLE, nullptr, nullptr);
	if (!_window)
		throw (string("Failed to create GLFW window"));
}

VeWindow::VeWindow(int width, int height) : _width(width), _height(height){
	initWindow();
}

VeWindow::~VeWindow(){
	glfwDestroyWindow(_window);
	glfwTerminate();
}

VkExtent2D	VeWindow::getExtent(void){
	return {static_cast<uint32_t>(_width), static_cast<uint32_t>(_height)};
}

bool		VeWindow::shouldClose(void){
	return (glfwWindowShouldClose(_window));
}

void		VeWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface){
	if (glfwCreateWindowSurface(instance, _window, nullptr, surface) != VK_SUCCESS)
		throw (runtime_error("failed to create window surface"));
}
