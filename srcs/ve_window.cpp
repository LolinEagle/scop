/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ve_window.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frrusso <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 15:21:45 by frrusso           #+#    #+#             */
/*   Updated: 2023/11/21 15:21:46 by frrusso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ve_window.hpp>

void	VeWindow::initWindow() {
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	_window = glfwCreateWindow(_width, _height, WINDOW_NAME, nullptr, nullptr);
}

VeWindow::VeWindow(void) : _width(1600), _height(900) {
	initWindow();
}

VeWindow::VeWindow(int width, int height) : _width(width), _height(height) {
	initWindow();
}

VeWindow::~VeWindow() {
	glfwDestroyWindow(_window);
	glfwTerminate();
}

void	VeWindow::run() {
	while (!glfwWindowShouldClose(_window)) {
		glfwPollEvents();
	}
}
