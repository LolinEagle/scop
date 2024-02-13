#pragma once

#include <VeModel.hpp>

using namespace std;

struct Transform2dComponent{
	glm::vec2	translation{};
	glm::vec2	scale{1.f, 1.f};
	float		rotation;

	glm::mat2	mat2(void);
};

class VeGameObject{
	private:
		int32_t					_id;

		VeGameObject(uint32_t id);
	public:
		shared_ptr<VeModel>		_model{};
		glm::vec3				_color{};
		Transform2dComponent	_transform2d{};

		VeGameObject(void);
		~VeGameObject();

		static VeGameObject	createGameObject(void);
		const uint32_t		getId(void);
};
