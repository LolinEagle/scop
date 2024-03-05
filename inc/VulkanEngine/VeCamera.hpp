#pragma once

#include <header.hpp>

using namespace std;

class VeCamera{
	private:
		glm::mat4	_projectionMatrix{1.f};
		glm::mat4	_viewMatrix{1.f};
		glm::mat4	_inverseViewMatrix{1.f};
	public:
		VeCamera(void);
		~VeCamera();

		void			setOrthographicProjection(
			float left, float right, float top, float bot, float near, float far
		);
		void			setPerspectiveProjection(float fovy, float aspect, float near, float far);
		void			setViewDirection(
			glm::vec3 position, glm::vec3 direction, glm::vec3 up = glm::vec3{0.f, -1.f, 0.f}
		);
		void			setViewTarget(
			glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3{0.f, -1.f, 0.f}
		);
		void			setViewYXZ(glm::vec3 position, glm::vec3 rotation);
		const glm::mat4	&getProjection(void) const;
		const glm::mat4	&getView(void) const;
		const glm::mat4	&getInverseView(void) const;
};
