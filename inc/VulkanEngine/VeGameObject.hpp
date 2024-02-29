#pragma once

#include <VeModel.hpp>

using namespace std;

struct TransformComponent{
	glm::vec3	translation{};
	glm::vec3	scale{1.f, 1.f, 1.f};
	glm::vec3	rotation{};

	glm::mat4	mat4(void);
	glm::mat3	normalMatrix(void);	
};

class VeGameObject{
	private:
		uint32_t	_id;

		VeGameObject(uint32_t id);
	public:
		using Map = unordered_map<uint32_t, VeGameObject>;

		shared_ptr<VeModel>	_model{};
		glm::vec3			_color{};
		TransformComponent	_transform{};

		VeGameObject(void);
		~VeGameObject();

		static VeGameObject	createGameObject(void);
		const uint32_t		getId(void);
};
