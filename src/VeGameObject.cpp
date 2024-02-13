#include <VeGameObject.hpp>

glm::mat2	Transform2dComponent::mat2(void){
	const float	s = glm::sin(rotation);
	const float	c = glm::cos(rotation);
	glm::mat2	rotMatrix{{c, s}, {-s, c}};
	glm::mat2	scaleMat{{scale.x, .0f}, {.0f, scale.y}};
	return (rotMatrix * scaleMat);
}

VeGameObject::VeGameObject(uint32_t id) : _id(id){
}

VeGameObject::VeGameObject(void){
	*this = createGameObject();
}

VeGameObject::~VeGameObject(){
}

VeGameObject		VeGameObject::createGameObject(void){
	static uint32_t	currentId = 0;

	return (VeGameObject(currentId++));
}

const uint32_t		VeGameObject::getId(void){
	return (_id);
}
