#pragma once

#include <header.hpp>

class VeWindow{
	private:
		const int	_width;
		const int	_height;
		GLFWwindow	*_window;

		void	initWindow(void);
	public:
		VeWindow(int width, int height);
		~VeWindow();

		void	createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
		void	run(void);
};
