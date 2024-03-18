#pragma once

#include <VeGameObject.hpp>

class Controller{
	private:
		struct KeyMappings{
			int	moveForward = GLFW_KEY_W;
			int	moveLeft = GLFW_KEY_A;
			int	moveBackward = GLFW_KEY_S;
			int	moveRight = GLFW_KEY_D;
			int	escape = GLFW_KEY_ESCAPE;
		};

		const KeyMappings	_keys{};
		const float			_moveSpeed = 4.f;
		const float			_lookSpeed = 16.f;
		const double		_lookOffset = 2.;
	public:
		Controller(GLFWwindow *window);
		~Controller();

		void	moveInPlaneXZ(GLFWwindow *window, float dt, VeGameObject &gameObject);
};
