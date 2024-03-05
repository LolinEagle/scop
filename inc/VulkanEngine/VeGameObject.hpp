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

struct PointLightComponent{
	float	lightIntensity = 1.f;
};

class VeGameObject{
	private:
		uint32_t	_id;

		VeGameObject(uint32_t id);
	public:
		using Map = unordered_map<uint32_t, VeGameObject>;

		glm::vec3						_color{};
		TransformComponent				_transform{};
		shared_ptr<VeModel>				_model{};
		unique_ptr<PointLightComponent>	_pointLight = nullptr;

		VeGameObject(void);
		~VeGameObject();

		VeGameObject(const VeGameObject &) = delete;
		VeGameObject &operator=(const VeGameObject &) = delete;
		VeGameObject(VeGameObject &&) = default;
		VeGameObject &operator=(VeGameObject &&) = default;

		static VeGameObject	createGameObject(void);
		static VeGameObject	makePointLight(
			float intensity = 8.f, float radius = .1f, glm::vec3 color = glm::vec3(1.f)
		);
		const uint32_t		getId(void);
};
