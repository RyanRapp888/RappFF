#ifndef CAMERA_INCLUDED_H
#define CAMERA_INCLUDED_H

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

struct Camera
{
public:
	Camera(const glm::vec3& pos, float fov, float aspect, float zNear, float zFar)
	{
		m_projection = glm::perspective(fov, aspect, zNear, zFar);
		m_pos = pos;
		m_forward = glm::vec3(0.0f, 0.0f, -1.0f);
		m_up = glm::vec3(0.0f, 1.0f, 0.0f);
		
	}

	inline glm::mat4 GetViewProjection() const
	{
		return m_projection * glm::lookAt(m_pos, m_pos + m_forward, m_up);
	}

protected:
private:
	glm::mat4 m_projection;
	glm::vec3 m_pos;
	glm::vec3 m_forward;
	glm::vec3 m_up;
};

#endif
