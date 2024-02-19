#include <KeyboardController.hpp>

KeyboardController::KeyboardController(void){
}

KeyboardController::~KeyboardController(){
}

void	KeyboardController::moveInPlaneXZ(GLFWwindow *window, float dt, VeGameObject &gameObject){
	// Keyboard Look Input
	glm::vec3	rotate{0};
	if (glfwGetKey(window, _keys.lookRight) == GLFW_PRESS) rotate.y += 1.f;
	if (glfwGetKey(window, _keys.lookLeft) == GLFW_PRESS) rotate.y -= 1.f;
	if (glfwGetKey(window, _keys.lookUp) == GLFW_PRESS) rotate.x += 1.f;
	if (glfwGetKey(window, _keys.lookDown) == GLFW_PRESS) rotate.x -= 1.f;


	if (glm::dot(rotate, rotate) > numeric_limits<float>::epsilon())
		gameObject._transform.rotation += _lookSpeed * dt * glm::normalize(rotate);
	gameObject._transform.rotation.x = glm::clamp(gameObject._transform.rotation.x, -1.5f, 1.5f);
	gameObject._transform.rotation.y = glm::mod(gameObject._transform.rotation.y, TWO_PI);

	float			yaw = gameObject._transform.rotation.y;
	const glm::vec3	forwardDir{sin(yaw), 0.f, cos(yaw)};
	const glm::vec3	rightDir{forwardDir.z, 0.f, -forwardDir.x};
	const glm::vec3	upDir{0.f, -1.f, 0.f};

	// Keyboard Move Input
	glm::vec3	moveDir{0};
	if (glfwGetKey(window, _keys.moveForward) == GLFW_PRESS) moveDir += forwardDir;
	if (glfwGetKey(window, _keys.moveLeft) == GLFW_PRESS) moveDir -= rightDir;
	if (glfwGetKey(window, _keys.moveBackward) == GLFW_PRESS) moveDir -= forwardDir;
	if (glfwGetKey(window, _keys.moveRight) == GLFW_PRESS) moveDir += rightDir;
	if (glfwGetKey(window, _keys.moveUp) == GLFW_PRESS) moveDir += upDir;
	if (glfwGetKey(window, _keys.moveDown) == GLFW_PRESS) moveDir -= upDir;

	if (glm::dot(moveDir, moveDir) > numeric_limits<float>::epsilon())
		gameObject._transform.translation += _moveSpeed * dt * glm::normalize(moveDir);
}
