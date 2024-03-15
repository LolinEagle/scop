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

		void			setPerspectiveProjection(float fovy, float aspect, float near, float far);
		void			setViewYXZ(glm::vec3 position, glm::vec3 rotation);
		const glm::mat4	&getProjection(void) const;
		const glm::mat4	&getView(void) const;
		const glm::mat4	&getInverseView(void) const;
		const glm::vec3	getPosition(void) const;
};
