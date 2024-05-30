#include <VeCamera.hpp>

VeCamera::VeCamera(void){
}

VeCamera::~VeCamera(){
}

void			VeCamera::setPerspectiveProjection(float fovy, float aspect, float near, float far){
	const float	tanHalfFovy = tan(fovy / 2.f);

	_projectionMatrix = glm::mat4{0.0f};
	_projectionMatrix.mat[0][0] = 1.f / (aspect * tanHalfFovy);
	_projectionMatrix.mat[1][1] = 1.f / (tanHalfFovy);
	_projectionMatrix.mat[2][2] = far / (far - near);
	_projectionMatrix.mat[2][3] = 1.f;
	_projectionMatrix.mat[3][2] = -(far * near) / (far - near);
}

void			VeCamera::setViewYXZ(glm::vec3 position, glm::vec3 rotation){
	const float	c3 = cos(rotation.z);
	const float	s3 = sin(rotation.z);
	const float	c2 = cos(rotation.x);
	const float	s2 = sin(rotation.x);
	const float	c1 = cos(rotation.y);
	const float	s1 = sin(rotation.y);
	const glm::vec3	u{(c1 * c3 + s1 * s2 * s3), (c2 * s3), (c1 * s2 * s3 - c3 * s1)};
	const glm::vec3	v{(c3 * s1 * s2 - c1 * s3), (c2 * c3), (c1 * c3 * s2 + s1 * s3)};
	const glm::vec3	w{(c2 * s1), (-s2), (c1 * c2)};

	_viewMatrix = glm::mat4{1.f};
	_viewMatrix.mat[0][0] = u.x;
	_viewMatrix.mat[1][0] = u.y;
	_viewMatrix.mat[2][0] = u.z;
	_viewMatrix.mat[0][1] = v.x;
	_viewMatrix.mat[1][1] = v.y;
	_viewMatrix.mat[2][1] = v.z;
	_viewMatrix.mat[0][2] = w.x;
	_viewMatrix.mat[1][2] = w.y;
	_viewMatrix.mat[2][2] = w.z;
	_viewMatrix.mat[3][0] = -glm::dot(u, position);
	_viewMatrix.mat[3][1] = -glm::dot(v, position);
	_viewMatrix.mat[3][2] = -glm::dot(w, position);

	_inverseViewMatrix = glm::mat4{1.f};
	_inverseViewMatrix.mat[0][0] = u.x;
	_inverseViewMatrix.mat[0][1] = u.y;
	_inverseViewMatrix.mat[0][2] = u.z;
	_inverseViewMatrix.mat[1][0] = v.x;
	_inverseViewMatrix.mat[1][1] = v.y;
	_inverseViewMatrix.mat[1][2] = v.z;
	_inverseViewMatrix.mat[2][0] = w.x;
	_inverseViewMatrix.mat[2][1] = w.y;
	_inverseViewMatrix.mat[2][2] = w.z;
	_inverseViewMatrix.mat[3][0] = position.x;
	_inverseViewMatrix.mat[3][1] = position.y;
	_inverseViewMatrix.mat[3][2] = position.z;
}

const glm::mat4	&VeCamera::getProjection(void) const {
	return (_projectionMatrix);
}

const glm::mat4	&VeCamera::getView(void) const {
	return (_viewMatrix);
}

const glm::mat4	&VeCamera::getInverseView(void) const {
	return (_inverseViewMatrix);
}

const glm::vec3	VeCamera::getPosition(void) const {
	return (glm::vec3(
		_inverseViewMatrix.mat[0][3], _inverseViewMatrix.mat[1][3], _inverseViewMatrix.mat[2][3]
	));
}
