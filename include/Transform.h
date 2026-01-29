#ifndef TRANSFORM_INCLUDED_H
#define TRANSFORM_INCLUDED_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // stable transform helpers
#include "camera.h"

struct Transform
{
public:

	Transform():m_pos(glm::vec3(0,0,0)),
		        m_rot(glm::vec3(0,0,0)),
				m_scale(glm::vec3(1, 1, 1)){}

	// rot is stored in radians
	Transform(const glm::vec3& pos,
		      const glm::vec3& rot,    // radians
		      const glm::vec3& scale) :m_pos(pos), m_rot(rot), m_scale(scale){}

	inline glm::mat4 GetModel() const
	{
		// Use the stable gtc::matrix_transform functions that take an initial matrix.
		glm::mat4 posMat   = glm::translate(glm::mat4(1.0f), m_pos);
		glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), m_scale);
		glm::mat4 rotX     = glm::rotate(glm::mat4(1.0f), m_rot.x, glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 rotY     = glm::rotate(glm::mat4(1.0f), m_rot.y, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 rotZ     = glm::rotate(glm::mat4(1.0f), m_rot.z, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 rotMat   = rotX * rotY * rotZ;
		return posMat * rotMat * scaleMat;
	}

	inline glm::mat4 GetMVP(glm::mat4 VP) const
	{
		glm::mat4 M = GetModel();
		return VP * M;
	}

	// Setters: SetRot expects radians.
	inline void SetPos(const glm::vec3& pos) { m_pos = pos; }
	inline void SetRot(const glm::vec3& rotRadians) { m_rot = rotRadians; }
	// Convenience: accept degrees and convert to radians.
	inline void SetRotDegrees(const glm::vec3& rotDegrees) { m_rot = glm::radians(rotDegrees); }
	inline void SetScale(const glm::vec3& scale) { m_scale = scale; }

protected:
private:
	glm::vec3 m_pos;
	glm::vec3 m_rot;   // angles (radians)
	glm::vec3 m_scale;
};

#endif
