#include <KeyboardController.hpp>

double	gScroll = 0;

void	scrollCallback(GLFWwindow *window, double xoffset, double yoffset){
	gScroll = yoffset;
}

KeyboardController::KeyboardController(GLFWwindow *window){
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetScrollCallback(window, scrollCallback);
}

KeyboardController::~KeyboardController(){
}

void	KeyboardController::moveInPlaneXZ(GLFWwindow *window, float dt, VeGameObject &gameObject){
	// Escape
	if (glfwGetKey(window, _keys.escape) == GLFW_PRESS) glfwSetWindowShouldClose(window, GLFW_TRUE);

	double		newXpos, newYpos;	// New cursor position
	glm::vec3	rotate{0};			// Rotate camera

	// Mouse input
	glfwGetCursorPos(window, &newXpos, &newYpos);
	if (newXpos < 0 || newYpos < 0 || newXpos > WIDTH || newYpos > HEIGHT)// If cursor not in window
		return ;
	if (HEIGHT_HALF > newYpos + _lookOffset) rotate.x += 1.f;	// Look up
	if (HEIGHT_HALF < newYpos - _lookOffset) rotate.x -= 1.f;	// Look down
	if (WIDTH_HALF < newXpos + _lookOffset) rotate.y += 1.f;	// Look right
	if (WIDTH_HALF > newXpos - _lookOffset) rotate.y -= 1.f;	// Look left
	glfwSetCursorPos(window, WIDTH_HALF, HEIGHT_HALF);			// Lock cursor

	// Rotation
	if (glm::dot(rotate, rotate) > numeric_limits<float>::epsilon())
		gameObject._transform.rotation += _lookSpeed * dt * glm::normalize(rotate);
	gameObject._transform.rotation.x = glm::clamp(gameObject._transform.rotation.x, -1.5f, 1.5f);
	gameObject._transform.rotation.y = glm::mod(gameObject._transform.rotation.y, TWO_PI);

	float			yaw = gameObject._transform.rotation.y;
	const glm::vec3	forwardDir{sin(yaw), 0.f, cos(yaw)};		// Forward direction
	const glm::vec3	rightDir{forwardDir.z, 0.f, -forwardDir.x};	// Right direction
	const glm::vec3	upDir{0.f, -1.f, 0.f};						// Up direction
	glm::vec3		moveDir{0};									// Move direction

	// Keyboard input
	if (glfwGetKey(window, _keys.moveForward) == GLFW_PRESS) moveDir += forwardDir;	// Move forward
	if (glfwGetKey(window, _keys.moveLeft) == GLFW_PRESS) moveDir -= rightDir;		// Move left
	if (glfwGetKey(window, _keys.moveBackward) == GLFW_PRESS) moveDir -= forwardDir;// Move backward
	if (glfwGetKey(window, _keys.moveRight) == GLFW_PRESS) moveDir += rightDir;		// Move right
	if (gScroll > 0){
		moveDir += upDir;// Move up
		gScroll -= 0.1;
	}
	if (gScroll < 0){
		moveDir -= upDir;// Move down
		gScroll += 0.1;
	}

	// Translation
	if (glm::dot(moveDir, moveDir) > numeric_limits<float>::epsilon())
		gameObject._transform.translation += _moveSpeed * dt * glm::normalize(moveDir);
}
