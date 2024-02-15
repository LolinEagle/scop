#include <VeCamera.hpp>

VeCamera::VeCamera(void){
}

VeCamera::~VeCamera(){
}

void			VeCamera::setOrthographicProjection(
	float left, float right, float top, float bot, float near, float far
){
	_projectionMatrix = glm::mat4{1.0f};
	_projectionMatrix[0][0] = 2.f / (right - left);
	_projectionMatrix[1][1] = 2.f / (bot - top);
	_projectionMatrix[2][2] = 1.f / (far - near);
	_projectionMatrix[3][0] = -(right + left) / (right - left);
	_projectionMatrix[3][1] = -(bot + top) / (bot - top);
	_projectionMatrix[3][2] = -near / (far - near);
}

void			VeCamera::setPerspectiveProjection(float fovy, float aspect, float near, float far){
	const float	tanHalfFovy = tan(fovy / 2.f);

	_projectionMatrix = glm::mat4{0.0f};
	_projectionMatrix[0][0] = 1.f / (aspect * tanHalfFovy);
	_projectionMatrix[1][1] = 1.f / (tanHalfFovy);
	_projectionMatrix[2][2] = far / (far - near);
	_projectionMatrix[2][3] = 1.f;
	_projectionMatrix[3][2] = -(far * near) / (far - near);
}

const glm::mat4	&VeCamera::getProjection(void) const {
	return (_projectionMatrix);
}
