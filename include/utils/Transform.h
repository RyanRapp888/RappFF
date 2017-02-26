#ifndef TRANSFORM_INCLUDED_H
#define TRANSFORM_INCLUDED_H

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "camera.h"

struct Transform
{
public:

	Transform():m_pos(glm::vec3(0,0,0)), 
		        m_rot(glm::vec3(0,0,0)), 
				m_scale(glm::vec3(1, 1, 1)){}

	Transform(const glm::vec3& pos,
		      const glm::vec3& rot,
		      const glm::vec3& scale) :m_pos(pos), m_rot(rot), m_scale(glm::vec3(1, 1, 1)){}

	inline glm::mat4 GetModel() const
	{
		glm::mat4 posMat = glm::translate(m_pos);
		glm::mat4 scaleMat = glm::scale(m_scale);
		glm::mat4 rotX = glm::rotate(m_rot.x, glm::vec3(1.0, 0.0, 0.0));
		glm::mat4 rotY = glm::rotate(m_rot.y, glm::vec3(0.0, 1.0, 0.0));
		glm::mat4 rotZ = glm::rotate(m_rot.z, glm::vec3(0.0, 0.0, 1.0));
		glm::mat4 rotMat = rotX * rotY * rotZ;
		return posMat * rotMat * scaleMat;
	}

	inline glm::mat4 GetMVP( glm::mat4 VP) const
	{
		glm::mat4 M = GetModel();
		return VP * M;
	}

	inline void SetPos(glm::vec3& pos) { m_pos = pos; }
	inline void SetRot(glm::vec3& rot) { m_rot = rot; }
	inline void SetScale(glm::vec3& scale) { m_scale = scale; }

protected:
private:
	glm::vec3 m_pos;
	glm::vec3 m_rot;
	glm::vec3 m_scale;
};

#endif
