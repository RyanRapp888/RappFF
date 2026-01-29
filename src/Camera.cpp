#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(const glm::vec3& pos, float fov, float aspect, float zNear, float zFar)
{
	// glm::perspective expects fovy in radians.
	m_projection = glm::perspective(glm::radians(fov), aspect, zNear, zFar);
	m_pos = pos;
	m_forward = glm::vec3(0.0f, 0.0f, -1.0f);
	m_up = glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::mat4 Camera::GetViewProjection() const
{
	return m_projection * glm::lookAt(m_pos, m_pos + m_forward, m_up);
}
