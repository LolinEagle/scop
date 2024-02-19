#pragma once

#include <VeGameObject.hpp>
#include <VeWindow.hpp>

using namespace std;

class KeyboardController{
	private:
		/* data */
	public:
		struct KeyMappings{
			int	moveForward = GLFW_KEY_W;
			int	moveLeft = GLFW_KEY_A;
			int	moveBackward = GLFW_KEY_S;
			int	moveRight = GLFW_KEY_D;
			int	moveDown = GLFW_KEY_Q;
			int	moveUp = GLFW_KEY_E;
			int	lookUp = GLFW_KEY_UP;
			int	lookLeft = GLFW_KEY_LEFT;
			int	lookDown = GLFW_KEY_DOWN;
			int	lookRight = GLFW_KEY_RIGHT;
		};

		KeyMappings	_keys{};
		float		_moveSpeed = 3.f;
		float		_lookSpeed = 1.5f;
		
		KeyboardController(void);
		~KeyboardController();

		void	moveInPlaneXZ(GLFWwindow *window, float dt, VeGameObject &gameObject);
};
