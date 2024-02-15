#pragma once

#include <header.hpp>

using namespace std;

class VeCamera{
	private:
		glm::mat4	_projectionMatrix{1.f};
	public:
		VeCamera(void);
		~VeCamera();

		void			setOrthographicProjection(
			float left, float right, float top, float bot, float near, float far
		);
		void			setPerspectiveProjection(float fovy, float aspect, float near, float far);
		const glm::mat4	&getProjection(void) const;
};
