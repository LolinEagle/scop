/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ve_window.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frrusso <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 15:21:31 by frrusso           #+#    #+#             */
/*   Updated: 2023/11/21 15:21:33 by frrusso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VE_WINDOW
#define VE_WINDOW

#include <header.hpp>

// GLFW
#include <GLFW/glfw3.h>
#define GLFW_INCLUDE_VULKAN

class VeWindow {
	private:
		const int	_width;
		const int	_height;
		GLFWwindow	*_window;

		void	initWindow();
	public:
		VeWindow(void);
		VeWindow(int width, int height);
		VeWindow(VeWindow const &) = delete;
		~VeWindow();

		VeWindow& operator=(VeWindow const &) = delete;

		void	run();
};

#endif
